#ifndef ud_server_hpp
#define ud_server_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <arpa/inet.h>

class ud_http_acceptor
{
private:
    /* data */
public:
    ud_http_acceptor(/* args */);
    ~ud_http_acceptor();
};

ud_http_acceptor::ud_http_acceptor(/* args */)
{
}

ud_http_acceptor::~ud_http_acceptor()
{
}


#endif