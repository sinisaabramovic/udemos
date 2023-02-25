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

#include  "ud_blacklist_manager.hpp"

class ud_http_acceptor
{
public:
    ud_http_acceptor(int port, std::shared_ptr<ud_blacklist_manager> blacklist_manager);
    ud_http_acceptor(int port) : m_port(port) 
    {
        initialize_socket();
    }
    ~ud_http_acceptor();

    int32_t accept_connection();    

private:
    void initialize_socket();    
    int32_t m_socket;    
    int m_port;
    std::shared_ptr<ud_blacklist_manager> m_blacklist_manager;
};

ud_http_acceptor::ud_http_acceptor(int port, std::shared_ptr<ud_blacklist_manager> blacklist_manager)
 : m_socket(-1),
  m_port(port),
  m_blacklist_manager(blacklist_manager)
{
    initialize_socket();
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
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int32_t new_socket;

    // Accept a new connection
    if ((new_socket = accept(m_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        return -1;
    }

    // Check if the client IP is blacklisted
    if (m_blacklist_manager) {
        if (m_blacklist_manager->is_ip_blacklisted(inet_ntoa(address.sin_addr)))
        {
            close(new_socket);
            return -1;
        }
    }    

    return new_socket;
}

void ud_http_acceptor::initialize_socket()
{
    struct sockaddr_in address;
    int opt = 1;

    // Create a TCP socket
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(m_port);

    if (bind(m_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(m_socket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

#endif
