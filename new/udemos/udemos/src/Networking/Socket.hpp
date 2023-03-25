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

class Socket {
public:
    Socket();
    Socket(int fd);
    ~Socket();
    
    void bind(const std::string& address, uint16_t port);
    void listen(int backlog = SOMAXCONN);
    int connect(const sockaddr* addr, socklen_t addrlen);
    void close();
    int fd() const;
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    
private:
    int socket_fd_;
    struct sockaddr_in addr_;
};

#endif /* Socket_hpp */
