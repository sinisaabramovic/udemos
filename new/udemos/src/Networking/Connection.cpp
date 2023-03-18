//
//  Connection.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "Connection.hpp"

Connection::Connection(EventLoop& loop, int fd)
: loop_(loop), socket_(std::make_shared<Socket>(loop, fd)) {
}

Connection::Connection(EventLoop& loop, const std::string& host, uint16_t port)
: loop_(loop) {
    try {
        socket_ = std::make_shared<Socket>(loop);
    } catch (const std::exception& ex) {
        throw std::runtime_error("Failed to create socket: " + std::string(ex.what()));
    }
    
    std::vector<std::string> addresses;
    try {
        addresses = AddressResolver::resolve(host);
    } catch (const std::exception& ex) {
        throw std::runtime_error("Failed to resolve address: " + std::string(ex.what()));
    }
    
    if (addresses.empty()) {
        throw std::runtime_error("No IP addresses found for host");
    }
    
    for (const auto& address : addresses) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
        
        try {
            if (socket_->connect(reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0) {
                return;
            }
        } catch (const std::exception& ex) {
            throw std::runtime_error("Failed to connect to socket: " + std::string(ex.what()));
        }
        
    }
    
    throw std::runtime_error("Failed to connect socket: " + std::string(strerror(errno)));
}

Connection::~Connection() {}
