#include "udRouter.hpp"
#include "reactive/udFlux.hpp"
#include "UdHttpRequest.hpp"
#include "udHttpResponse.hpp"
#include "udConnection.hpp"

udConnection::udConnection(int socket, std::shared_ptr<udRouter> router)
    : socket_(socket), router_(router), request_stream_(std::make_shared<udRequestStream>())
{
}

void Start()
{
    std::cout << "Starting connection..." << std::endl;

    while (true)
    {
        try
        {
            std::string data;
            size_t bytes_read = recv(socket_, data.data(), data.size(), 0);

            if (bytes_read == 0)
            {
                std::cout << "Connection closed" << std::endl;
                break;
            }
            else if (bytes_read == -1)
            {
                std::cerr << "Error reading from socket" << std::endl;
                break;
            }
            else
            {
                request_stream_->write(data);
            }
        }
        catch (std::exception &ex)
        {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }
}

void udConnection::Send(std::shared_ptr<udHttpResponse> response)
{
    std::string data = response->ToString();
    send(socket_, data.c_str(), data.size(), 0);
}

void udConnection::Close()
{
    close(socket_);
}

void udConnection::update(const std::string &message) override
{
    auto request = std::make_shared<UdHttpRequest>(message);
    auto response = router_->HandleRequest(request);
    response_stream_->send(response->ToString());
}

void udConnection::attach_request_stream(std::shared_ptr<udRequestStream> request_stream)
{
    request_stream_ = request_stream;
    request_stream_->attach(shared_from_this());
}

void udConnection::attach_response_stream(std::shared_ptr<udResponseStream> response_stream)
{
    response_stream_ = response_stream;
}