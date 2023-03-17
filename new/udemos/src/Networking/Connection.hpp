//
//  Connection.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef Connection_hpp
#define Connection_hpp

#include <memory>
#include <string>
#include <stdexcept>
#include <cstring> // For strerror
#include <cerrno>  // For errno
#include "Socket.hpp"
#include "AddressResolver.hpp"
#include "EventLoop.h"

class Connection {
public:
    Connection(EventLoop& loop, const std::string& host, uint16_t port);
    ~Connection();

    // Non-copyable
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    EventLoop& loop_;
    std::shared_ptr<Socket> socket_;
};

#endif /* Connection_hpp */
