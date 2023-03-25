#ifndef ud_http_server_hpp
#define ud_http_server_hpp

#include <thread>
#include <chrono>
#include <atomic>
#include <random>
#include <memory>

#include "ud_server_data.hpp"
#include "ud_server_utils.hpp"

class ud_http_server
{
public:
    ud_http_server() : data_ptr(new ud_server_data())
    {
        data_ptr->set_port(8080);
        data_ptr->set_host("0.0.0.0");
        data_ptr->setup_socket();
    }

    ud_http_server(uint32_t port, const std::string &host) : data_ptr(new ud_server_data())
    {
        data_ptr->set_port(port);
        data_ptr->set_host(host);
        data_ptr->setup_socket();
    }

    ~ud_http_server() {}

    void pause_listen(bool pause)
    {
        std::unique_lock<std::mutex> lock(data_ptr->get_pause_mutex());
        data_ptr->set_pause_flag(pause);
        data_ptr->get_pause_cv().notify_one();
        data_ptr->set_stoped(pause);
    }

    void stop_listen()
    {
        data_ptr->set_stop_flag(true);
        data_ptr->set_stoped(true);   
        data_ptr->close();     

        data_ptr->get_thread_pool()->stop();
        if (data_ptr->get_listener_thread())
        {
            data_ptr->get_listener_thread()->join();
        }        
    }

    bool is_running()
    {
        return !data_ptr->get_stoped();
    }

    bool is_paused()
    {
        return !data_ptr->get_pause_flag();
    }

    void start_listen(std::shared_ptr<ud_http_router> router)
    {
        data_ptr->set_router(std::move(router));
        data_ptr->set_thread_pool(std::make_unique<ud_http_thread_pool>(32));
        data_ptr->set_acceptor(std::make_unique<ud_http_acceptor>(data_ptr->get_port(), data_ptr->get_sock_fd()));

        data_ptr->get_acceptor()->initialize_socket(TIMEOUT_DELAY, data_ptr->get_host());

        data_ptr->set_listener_thread(std::make_unique<std::thread>(&ud_server_utils::listen, std::ref(*data_ptr)));
        data_ptr->get_listener_thread()->join();
    }

private:
    std::unique_ptr<ud_server_data> data_ptr;
};

#endif