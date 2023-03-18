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
    socket_ = std::make_shared<Socket>(loop);
    auto addresses = AddressResolver::resolve(host);
    
    if (addresses.empty()) {
        throw std::runtime_error("No IP addresses found for host");
    }
    
    for (const auto& address : addresses) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
        
        if (socket_->connect(reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0) {
            return;
        }
    }
    
    // If connect() has failed for all resolved addresses, throw an error with a detailed message
    throw std::runtime_error("Failed to connect socket: " + std::string(strerror(errno)));
}

Connection::~Connection() {
}
