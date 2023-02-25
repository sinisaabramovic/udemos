#ifndef udp_http_acceptor_hpp
#define udp_http_acceptor_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <arpa/inet.h>

#include "../common/udp_http_defines.h"
#include "ud_blacklist_manager.hpp"

class ud_http_acceptor
{
public:
    ud_http_acceptor(int32_t port, int32_t m_sock_fd, std::shared_ptr<ud_blacklist_manager> blacklist_manager);
    ud_http_acceptor(int32_t port, int32_t sock_fd) : m_port(port), m_socket(sock_fd)
    {
    }
    ~ud_http_acceptor();

    void initialize_socket(int32_t sock_fd, uint32_t port, const std::string &host);
    int32_t accept_connection();

private:
    int32_t m_socket;
    int32_t m_port;
    std::shared_ptr<ud_blacklist_manager> m_blacklist_manager;
};

ud_http_acceptor::ud_http_acceptor(int port, int32_t sock_fd, std::shared_ptr<ud_blacklist_manager> blacklist_manager)
    : m_socket(sock_fd),
      m_port(port),
      m_blacklist_manager(blacklist_manager)
{
}

ud_http_acceptor::~ud_http_acceptor()
{
    if (m_socket >= 0)
    {
        close(m_socket);
    }
}

int32_t ud_http_acceptor::accept_connection()
{
    sockaddr_in address;
    int addrlen = sizeof(address);
    int32_t client_socket;

    // Accept a new connection
    if ((client_socket = accept(m_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) <= 0)
    {
        perror("accept");
        close(client_socket);
        return -1;
    }

    std::cout << "ip: " << inet_ntoa(address.sin_addr) << std::endl;

    // Check if the client IP is blacklisted
    if (m_blacklist_manager)
    {
        if (m_blacklist_manager->is_ip_blacklisted(inet_ntoa(address.sin_addr)))
        {
            close(client_socket);
            return -1;
        }
    }

    return client_socket;
}

void ud_http_acceptor::initialize_socket(int32_t sock_fd, uint32_t port, const std::string &host)
{
    std::cout << sock_fd << " " << port << " " << host << std::endl;
    int opt = 1;
    sockaddr_in server_address;

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
                   sizeof(opt)) < 0)
    {
        throw std::runtime_error("Failed to set socket options");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, host.c_str(), &(server_address.sin_addr.s_addr));
    server_address.sin_port = htons(port);

    if (bind(sock_fd, (sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        throw std::runtime_error("Failed to bind to socket");
    }

    if (listen(sock_fd, MAX_BACKLOG_SIZE) < 0)
    {
        std::ostringstream msg;
        msg << "Failed to listen on port " << port;
        throw std::runtime_error(msg.str());
    }
}

#endif
