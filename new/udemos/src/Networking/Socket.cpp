//
//  Socket.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <fcntl.h>
#include <unistd.h> // For close
#include <fcntl.h>  // For fcntl
#include <cstring>  // For strerror
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "Socket.hpp"

Socket::Socket(EventLoop& loop, int fd) : loop_(loop), socket_fd_(fd) {
    
}

Socket::Socket(EventLoop& loop) : loop_(loop) {
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    
    // Set the socket to non-blocking mode
#ifdef __linux__
    int flags = fcntl(socket_fd_, F_GETFL, 0);
    fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK);
#elif __APPLE__
    int flags = fcntl(socket_fd_, F_GETFL, 0);
    fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK);
#endif
}

Socket::~Socket() {
    close();
}

int Socket::fd() const {
    return socket_fd_;
}

void Socket::bind(const std::string& address, uint16_t port) {
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(address.c_str());
    addr_.sin_port = htons(port);
    
    constexpr int maxRetries = 128;
    int attempts = 0;
    
    while (attempts < maxRetries) {
        int bindResult = ::bind(socket_fd_, (struct sockaddr*)&addr_, sizeof(addr_));
        if (bindResult >= 0) {
            return;
        }
        
        attempts++;
        std::cerr << "Failed to bind socket, attempt " << attempts << ": " << strerror(errno) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    throw std::runtime_error("Failed to bind socket after " + std::to_string(maxRetries) + " attempts");
}

void Socket::listen(int backlog) {
    if (::listen(socket_fd_, backlog) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

int Socket::connect(const sockaddr* addr, socklen_t addrlen) {
    int result = ::connect(socket_fd_, addr, addrlen);
    
    if (result < 0 && errno != EINPROGRESS) {
        throw std::runtime_error("Failed to connect socket: " + std::string(strerror(errno)));
    }
    
    return result;
}

void Socket::close() {
    if (socket_fd_ >= 0) {
        ::close(socket_fd_);
        socket_fd_ = -1;
    }
}
