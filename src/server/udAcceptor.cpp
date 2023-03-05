#include "udRouter.hpp"
#include "udThreadPool.hpp"
#include "udAcceptor.hpp"

udAcceptor::udAcceptor(int port, std::shared_ptr<udRouter> router, size_t num_threads)
    : port_(port), running_(false), router_(router), thread_pool_(num_threads)
{
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == -1)
    {
        throw std::runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        throw std::runtime_error("Failed to set socket option");
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(server_fd_, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        throw std::runtime_error("Failed to bind socket to port");
    }

    if (listen(server_fd_, 10) < 0)
    {
        throw std::runtime_error("Failed to listen for incoming connections");
    }
}

void udAcceptor::start()
{
    running_ = true;

    while (running_)
    {
        int client_fd;
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        client_fd = accept(server_fd_, (sockaddr *)&client_addr, &client_len);
        if (client_fd < 0)
        {
            throw std::runtime_error("Failed to accept connection");
        }

        auto connection = std::make_shared<udConnection>(client_fd, router_);
        router_->attach_request_stream(connection->request_stream_);
        auto response_stream = std::make_shared<udResponseStream>();
        response_stream->attach(connection);
        connection->response_stream_ = response_stream;
        thread_pool_.submit([connection]()
                            { connection->Start(); });
    }
}

void udAcceptor::stop()
{
    running_ = false;
}