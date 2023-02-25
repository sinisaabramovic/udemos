#ifndef ud_http_hpp
#define ud_http_hpp

#include <thread>
#include <chrono>
#include <atomic>

#include "common/ud_thread_manager.hpp"
#include "acceptor/ud_http_acceptor.hpp"
#include "connection/ud_http_connection.hpp"
#include "router/ud_http_router.hpp"
#include "common/ud_http_thread_pool.hpp"
#include "ud_server.hpp"

class ud_http : public ud_server
{
private:
    u_int32_t m_port;
    std::shared_ptr<ud_http_router> m_router;
    std::unique_ptr<ud_http_acceptor> m_acceptor;
    std::unique_ptr<ud_http_thread_pool> m_thread_pool;

public:
    ud_http()
    {
        stop_flag = false;
    }
    ~ud_http() {}

    void pause_listen(bool pause) override
    {        
        std::unique_lock<std::mutex> lock(pause_mutex);
        pause_flag = pause;
        std::cout << "pause_listen: " << pause << std::endl;
        pause_cv.notify_one();
        stoped = pause;
    }

    void stop_listen() override
    {
        stop_flag = true;
        stoped = true;
    }

    bool is_running() override
    {
        return !stoped;
    }
    
    bool is_paused() override
    {
        return !paused;
    }
    
    void start_listen(
        u_int32_t port, 
        const std::string &host_name, 
        std::shared_ptr<ud_http_router> router,
        status_delegate delegate) override
    {
        using ud_result_type = ud_result<ud_result_success, ud_result_failure>;
        m_port = port;
        m_router = std::move(router);
        m_thread_pool = std::make_unique<ud_http_thread_pool>(4);
        m_acceptor = std::make_unique<ud_http_acceptor>(port);
        // Start a new thread
        std::thread listener_thread([&]()
        {
            bool success = bind_listener(port, host_name);
            if (!success) 
            {
                ud_result_type failure_listen{ud_result_failure{"Failed to bind listener to port."}};
                delegate(failure_listen);                
                stop_listen();
                return;
            }

            while (!stop_flag) 
            {
                std::unique_lock<std::mutex> lock(pause_mutex);
                pause_cv.wait(lock, [&]() {
                    return pause_flag.load() == false; 
                });
                lock.unlock();

                int32_t new_socket;
                if ((new_socket = accept_connection(delegate)) > 0) {
                    ud_result_type my_result{ud_result_success{"Incomming connection..."}};
                    delegate(my_result);
                    std::shared_ptr<ud_http_connection<ud_http_router>> client_connection = std::make_shared<ud_http_connection<ud_http_router>>(new_socket, m_router);
                    m_thread_pool->enqueue(&ud_http_connection<ud_http_router>::start, client_connection);
                }
            } 
        });

        // Detach the thread so that it can continue running in the background
        listener_thread.detach();
    }

private:
    bool bind_listener(u_int32_t port, const std::string &host_name)
    {
        // TODO: Implement code to bind listener to port and host_name
        // Return true if successful, false otherwise
        std::cout << "bind_listener() \n";
        return true;
    }

    void unbind_listener()
    {
        return;
    }

    int32_t accept_connection(status_delegate delegate)
    {
        using ud_result_type = ud_result<ud_result_success, ud_result_failure>;
        int32_t new_socket;
        if ((new_socket = m_acceptor->accept_connection()) < 0)
        {
            ud_result_type failure_listen{ud_result_failure{"Failed accept connection"}};
            delegate(failure_listen); 
            return -1;
        }
        return new_socket;
    }
};

#endif