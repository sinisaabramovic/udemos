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

class ud_http_acceptor
{
public:
    ud_http_acceptor(int32_t port, int32_t m_sock_fd, std::shared_ptr<ud_blacklist_manager> blacklist_manager);
    ud_http_acceptor(int32_t port, int32_t sock_fd) : m_port(port), m_socket(std::make_unique<ud_socket_wrapper>(sock_fd))
    {
    }
    ~ud_http_acceptor();

    using status_delegate = std::function<void(const ud_result<ud_result_success, ud_result_failure> &)>;
    void initialize_socket(int32_t sock_fd, int timeout_ms, uint32_t port, const std::string &host, status_delegate delegate);
    int32_t accept_connection(status_delegate delegate);

private:
    int32_t m_port;
    std::unique_ptr<ud_socket_wrapper> m_socket;
    std::shared_ptr<ud_blacklist_manager> m_blacklist_manager;
};

ud_http_acceptor::ud_http_acceptor(int port, int32_t sock_fd, std::shared_ptr<ud_blacklist_manager> blacklist_manager)
    : m_socket(std::make_unique<ud_socket_wrapper>(sock_fd)),
      m_port(port),
      m_blacklist_manager(blacklist_manager)
{
}

ud_http_acceptor::~ud_http_acceptor()
{
    if (m_socket->get_fd() >= 0)
    {
        close(m_socket->get_fd());
    }
}

int32_t ud_http_acceptor::accept_connection(status_delegate delegate)
{
    using ud_result_type = ud_result<ud_result_success, ud_result_failure>;
    sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int32_t client_socket = accept(m_socket->get_fd(), (struct sockaddr *)&address, &addrlen);

    if (client_socket < 0)
    {
        auto result = ud_result_type{ud_result_success{"ERROR: incoming connection - accept"}};
        delegate(result);
        return -1;
    }

    std::string client_ip_info = "Incoming connection from client ip: " + std::string(inet_ntoa(address.sin_addr));
    auto result = ud_result_type{ud_result_success{client_ip_info}};
    delegate(result);

    if (m_blacklist_manager && m_blacklist_manager->is_ip_blacklisted(inet_ntoa(address.sin_addr)))
    {
        std::string client_ip_info = "REJECTED (blacklisted): Incoming connection from client ip: " + std::string(inet_ntoa(address.sin_addr));
        auto result = ud_result_type{ud_result_success{client_ip_info}};
        delegate(result);
        close(client_socket);
        return -1;
    }

    return client_socket;
}

void ud_http_acceptor::initialize_socket(int32_t sock_fd, int timeout_ms, uint32_t port, const std::string &host, status_delegate delegate)
{
    using ud_result_type = ud_result<ud_result_success, ud_result_failure>;
    std::string info_message = "initialize_socket(): " + host;
    auto result = ud_result_type{ud_result_success{info_message}};
    delegate(result);
    int opt = 1;
    sockaddr_in server_address;

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        throw std::runtime_error("Failed to set socket options");
        auto result = ud_result_type{ud_result_success{"Failed to set socket options"}};
        delegate(result);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, host.c_str(), &(server_address.sin_addr.s_addr));
    server_address.sin_port = htons(port);

    if (bind(sock_fd, (sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        throw std::runtime_error("Failed to bind to socket");
        auto result = ud_result_type{ud_result_success{"initialize_socket() Failed to bind to socket"}};
        delegate(result);
    }

    if (listen(sock_fd, MAX_BACKLOG_SIZE) < 0)
    {
        std::ostringstream msg;
        msg << "Failed to listen on port " << port;
        throw std::runtime_error(msg.str());
        auto result = ud_result_type{ud_result_success{"initialize_socket() Failed to listen on port"}};
        delegate(result);
    }
}

#endif
