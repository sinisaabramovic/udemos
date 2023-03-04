#ifndef ud_http_service_hpp
#define ud_http_service_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <functional>

#include "../ud_service.hpp"
#include "../router/ud_router.hpp"
#include "../connection/ud_connection.hpp"
#include "../router/ud_router.hpp"
#include "../reactive/ud_reactive.hpp"
#include "../reactive/ud_subscriber.hpp"
#include "../models/request/ud_http_request.hpp"
#include "../models/response/ud_http_response.hpp"

template <typename Protocol>
class ud_http_service : public ud_service<Protocol>, public std::enable_shared_from_this<ud_http_service<Protocol>>
{
public:
    ud_http_service(int port) : port_(port), router_(std::make_shared<ud_router<Protocol>>()) {}
    void start() override
    {
        // Create socket and bind to port
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            throw std::runtime_error("Failed to create socket");
        }
        sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port_);
        int result = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
        if (result < 0)
        {
            throw std::runtime_error("Failed to bind to port");
        }
        // Listen for incoming connections
        result = listen(sockfd, 10);
        if (result < 0)
        {
            throw std::runtime_error("Failed to listen on port");
        }
        // Accept connections and spawn threads to handle them
        while (true)
        {
            int connfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
            if (connfd < 0)
            {
                throw std::runtime_error("Failed to accept connection");
            }
            auto connection = std::make_shared<ud_connection<Protocol>>(connfd, router_);
            connections_.push_back(connection);
            auto request_stream = connection->get_request_stream();
            request_stream->subscribe(shared_from_this());
            std::thread(&ud_connection<Protocol>::run, connection).detach();
        }
    }
    void stop() override
    {
        // Close all connections
        for (auto connection : connections_)
        {
            close(connection->socket_);
        }
        connections_.clear();
    }
    void add_route(std::shared_ptr<ud_route<Protocol>> route)
    {
        router_->add_route(route);
    }
    void on_request(std::shared_ptr<ud_http_request<Protocol>> request) override
    {
        // Handle incoming request
        auto response_stream = std::make_shared<ud_response_stream<Protocol>>();
        router_->handle_request(request, response_stream);
        response_stream->subscribe(shared_from_this());
    }
    void on_response(std::shared_ptr<ud_http_response<Protocol>> response) override
    {
        // Send response to client
        for (auto connection : connections_)
        {
            int n = send(connection->socket_, response->get_raw_data().c_str(), response->get_raw_data().length(), 0);
            if (n < 0)
            {
                throw std::runtime_error("Failed to send response");
            }
        }
    }

private:
    int port_;
    std::shared_ptruudrrouter<Protocol>> router_;
    std::vector<std::shared_ptr<dd_connection<Protocol>>> connections_;
};

#endif
