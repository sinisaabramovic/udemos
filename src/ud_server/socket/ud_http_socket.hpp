#ifndef ud_http_socket_hpp
#define ud_http_socket_hpp

#include "ud_socket.hpp"

class ud_http_socket : public ud_socket
{

public:
    ud_http_socket(int32_t socket) : ud_socket(socket) {}

    int32_t get_socket() const override { return m_socket; }
    void close_socket() override { close(m_socket); }
};

#endif