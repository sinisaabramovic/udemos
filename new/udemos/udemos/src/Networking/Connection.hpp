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
#include <cstring>
#include <cerrno>

#include "Socket.hpp"
#include "AddressResolver.hpp"

class Connection {
public:
    Connection(const std::string& host, uint16_t port);
    Connection(int fd);
    ~Connection();

    Socket& socket() { return *socket_; }
    void setKeepAlive(bool keep_alive);
    bool keepAlive() const;
    // Non-copyable
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    std::shared_ptr<Socket> socket_;
    bool keep_alive_;
};

#endif /* Connection_hpp */
