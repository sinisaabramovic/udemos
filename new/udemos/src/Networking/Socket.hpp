//
//  Socket.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef Socket_hpp
#define Socket_hpp

#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "EventLoop.h"

class Socket {
public:
    Socket(EventLoop& loop);
    Socket(EventLoop& loop, int fd);
    ~Socket();
    
    void bind(const std::string& address, uint16_t port);
    void listen(int backlog = 5);
    int connect(const sockaddr* addr, socklen_t addrlen);
    void close();
    int fd() const;
    
    // Non-copyable
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    
private:
    EventLoop& loop_;
    int socket_fd_;
    struct sockaddr_in addr_;
};

#endif /* Socket_hpp */
