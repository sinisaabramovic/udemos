#ifndef ud_socket_hpp
#define ud_socket_hpp

#include <memory>
#include <string>
#include <sys/socket.h>

class ud_socket
{
protected:
    int32_t m_socket;

public:
    ud_socket(int32_t socket) : m_socket(socket) {}
    virtual ~ud_socket() {}

    virtual int32_t get_socket() const = 0;
    virtual void close_socket() = 0;
};

#endif