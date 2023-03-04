#ifndef UD_ACCEPTOR_HPP
#define UD_ACCEPTOR_HPP

#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <memory>

class udRouter;
class udThreadPool;

class udAcceptor
{
public:
    udAcceptor(int port, std::shared_ptr<udRouter> router, size_t num_threads);
    void start();
    void stop();
private:
    int port_;
    int server_fd_;
    bool running_;
    std::shared_ptr<udRouter> router_;
    udThreadPool thread_pool_;
};

#endif // UD_ACCEPTOR_HPP
