#ifndef ud_https_service_hpp
#define ud_https_service_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <functional>

#include "../ud_service.hpp"

class ud_https_service : public ud_service<ud_https_service>
{
public:
    ud_https_service(uint32_t port, const std::string& host) : m_port(port), m_host(host), m_sock_fd(0)
    {}
    void start() override {}
    void stop() override {}

    bool is_running() override
    { 
        return !this->m_stoped;
    }

protected:
    std::atomic<bool> m_stop_flag = false;
    std::atomic<bool> m_pause_flag = false;
    bool m_stoped = false;
    std::condition_variable m_pause_cv;
    std::mutex m_pause_mutex;
    std::unique_ptr<std::thread> m_listener_thread;
    std::string m_host;
    uint32_t m_port;
    int32_t m_sock_fd;
};

#endif
