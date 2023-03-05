#ifndef ud_http_hpp
#define ud_http_hpp

#include <thread>
#include <chrono>
#include <atomic>
#include <random>

#include "common/ud_thread_manager.hpp"
#include "acceptor/ud_http_acceptor.hpp"
#include "connection/ud_http_connection.hpp"
#include "router/ud_http_router.hpp"
#include "common/ud_http_thread_pool.hpp"
#include "ud_server.hpp"

class ud_http : public ud_server
{
private:
    std::shared_ptr<ud_http_router> m_router;
    std::unique_ptr<ud_http_acceptor> m_acceptor;
    std::unique_ptr<ud_http_thread_pool> m_thread_pool;
    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_sleep_times;

public:
    ud_http() :
     ud_server(8080, "0.0.0.0"), 
     m_rng(std::chrono::steady_clock::now().time_since_epoch().count()),
     m_sleep_times(10, 100)
    {
        this->stop_flag = false;
        this->setup_socket();
    }
    ud_http(uint32_t port, const std::string& host) :
     ud_server(port, host), 
     m_rng(std::chrono::steady_clock::now().time_since_epoch().count()),
     m_sleep_times(10, 100)
    {
        this->stop_flag = false;
        this->setup_socket();
    }
    ~ud_http() {}

    void pause_listen(bool pause) override
    {
        std::unique_lock<std::mutex> lock(pause_mutex);
        this->pause_flag = pause;
        this->pause_cv.notify_one();
        this->stoped = pause;
    }

    void stop_listen() override
    {
        this->stop_flag = true;
        this->stoped = true;        
        if (this->m_listener_thread) 
        {
            this->m_listener_thread->join();
        }        
        this->m_thread_pool->stop();
    }

    bool is_running() override
    {
        return !this->stoped;
    }

    bool is_paused() override
    {
        return !this->paused;
    }

    void setup_socket()
    {
        if ((this->m_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
        {
            throw std::runtime_error("Failed to create a TCP socket");
        }
    }

    void start_listen(
        std::shared_ptr<ud_http_router> router,
        status_delegate delegate) override
    {        
        m_router = std::move(router);
        m_thread_pool = std::make_unique<ud_http_thread_pool>(32);
        m_acceptor = std::make_unique<ud_http_acceptor>(this->m_port, this->m_sock_fd);

        m_acceptor->initialize_socket(this->m_sock_fd, TIMEOUT_DELAY, this->m_port, this->m_host, delegate);

        m_listener_thread = std::make_unique<std::thread>(&ud_http::listen, this, delegate);
        m_listener_thread->join();
    }

private:
    void listen(status_delegate delegate)
    {       
        using ud_result_type = ud_result<ud_result_success, ud_result_failure>; 
        int client_socket_fd;
        bool active = true;        
        
        while (!this->stoped)
        {   
            if (!active) 
            {
                std::this_thread::sleep_for(std::chrono::microseconds(m_sleep_times(m_rng)));
            }       

            if ((client_socket_fd = m_acceptor->accept_connection(delegate)) <= 0)
            {                
                active = false;
                auto result = ud_result_type{ud_result_success{"listen accept rejected"}};
                delegate(result);
                close(client_socket_fd);               
                continue;
            }
            active = true;
            
            std::shared_ptr<ud_http_connection<ud_http_router>> client_connection = std::make_shared<ud_http_connection<ud_http_router>>(client_socket_fd, m_router);
            this->m_thread_pool->enqueue(&ud_http_connection<ud_http_router>::start, client_connection);
        }
    }
};

#endif