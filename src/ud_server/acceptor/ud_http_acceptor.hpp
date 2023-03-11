#ifndef ud_http_acceptor_hpp
#define ud_http_acceptor_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <arpa/inet.h>

#include "../common/ud_http_defines.h"
#include "ud_blacklist_manager.hpp"
#include "../common/ud_result.hpp"
#include "../common/ud_result_failure.hpp"
#include "../common/ud_result_success.hpp"
#include "../socket/ud_socket_wrapper.hpp"
#include "ud_acceptor_data.hpp"
#include "ud_acceptor_util.hpp"

class ud_http_acceptor
{
public:
    ud_http_acceptor(int32_t port, int32_t sock_fd, std::shared_ptr<ud_blacklist_manager> blacklist_manager);
    ud_http_acceptor(int32_t port, int32_t sock_fd) : ud_http_acceptor(port, sock_fd, nullptr) {}
    ~ud_http_acceptor();

    void initialize_socket(int timeout_ms, const std::string &host);

    int32_t accept_connection();

private:
    ud_acceptor_data m_acceptor_data;
    std::shared_ptr<ud_blacklist_manager> m_blacklist_manager;
    std::unique_ptr<ud_acceptor_util> m_acceptor_util;
};

ud_http_acceptor::ud_http_acceptor(int32_t port, int32_t sock_fd, std::shared_ptr<ud_blacklist_manager> blacklist_manager)
    : m_acceptor_data(port, sock_fd),
      m_blacklist_manager(blacklist_manager),
      m_acceptor_util(std::make_unique<ud_acceptor_util>(blacklist_manager))
{
}

ud_http_acceptor::~ud_http_acceptor()
{
    if (m_acceptor_data.get_socket_fd() >= 0)
    {
        close(m_acceptor_data.get_socket_fd());
    }
}

void ud_http_acceptor::initialize_socket(int timeout_ms, const std::string &host)
{
    m_acceptor_data = ud_acceptor_data(m_acceptor_data.get_port(), m_acceptor_data.get_socket_fd());
    m_blacklist_manager = std::make_shared<ud_blacklist_manager>();
    m_acceptor_util = std::make_unique<ud_acceptor_util>(m_blacklist_manager);

    m_acceptor_util->initialize_socket(m_acceptor_data.get_socket_fd(), timeout_ms, m_acceptor_data.get_port(), host);
}

int32_t ud_http_acceptor::accept_connection()
{
    return m_acceptor_util->accept_connection(m_acceptor_data.get_socket_fd());
}

#endif
