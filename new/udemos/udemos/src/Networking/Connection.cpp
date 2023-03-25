//
//  Connection.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "Connection.hpp"

Connection::Connection(int fd)
: socket_(std::make_shared<Socket>(fd)), keep_alive_(false)  {
}

Connection::Connection(const std::string& host, uint16_t port) {
    try {
        socket_ = std::make_shared<Socket>();
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

void Connection::setKeepAlive(bool keep_alive) {
    keep_alive_ = keep_alive;
}

bool Connection::keepAlive() const {
    return keep_alive_;
}

Connection::~Connection() {}
