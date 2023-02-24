#ifndef HTTP_ACCEPTOR_HPP
#define HTTP_ACCEPTOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <arpa/inet.h>

class HttpAcceptor
{
public:
    HttpAcceptor(int port, const std::vector<std::string> &blacklisted_ips = {});
    ~HttpAcceptor();

    int32_t accept_connection();
    void set_blacklisted_ip(const std::string &ip);

private:
    void initialize_socket();
    bool is_ip_blacklisted(const std::string &ip);

    int32_t m_socket;
    std::vector<std::string> m_blacklisted_ips;
    int m_port;
};

HttpAcceptor::HttpAcceptor(int port, const std::vector<std::string> &blacklisted_ips) : m_socket(-1), m_blacklisted_ips(blacklisted_ips), m_port(port)
{
    initialize_socket();
}

HttpAcceptor::~HttpAcceptor()
{
    if (m_socket >= 0)
    {
        close(m_socket);
    }
}

int32_t HttpAcceptor::accept_connection()
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
    if (is_ip_blacklisted(inet_ntoa(address.sin_addr)))
    {
        close(new_socket);
        return -1;
    }

    return new_socket;
}

void HttpAcceptor::set_blacklisted_ip(const std::string &ip)
{
    m_blacklisted_ips.emplace_back(ip);
}

void HttpAcceptor::initialize_socket()
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

bool HttpAcceptor::is_ip_blacklisted(const std::string &ip)
{
    return std::find(m_blacklisted_ips.begin(), m_blacklisted_ips.end(), ip) != m_blacklisted_ips.end();
}

#endif
