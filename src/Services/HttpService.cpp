//
//  BaseService.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/select.h>
#include <iostream>
#include <future>
#include <vector>
#include <algorithm>
#include <chrono>

#ifdef __APPLE__
#include <sys/event.h>
#endif

#include "../Core/Logger/Logger.hpp"
#include "ServiceType.hpp"
#include "../Protocol/HttpProtocolHandler.h"
#include "HttpService.hpp"
#include "../Networking/ConnectionPool.hpp"

HttpService::HttpService(const Configuration& config) : config_(config.getInstance())
{
    try {
        this->server_socket_ = std::make_shared<Socket>();
    } catch (const std::exception& ex) {
        Logger::getInstance().log(LogLevel::Error, "Failed to create BaseService: " + std::string(ex.what()));
    }
    this->stop_flag_ = false;
}
HttpService::~HttpService() {}

void HttpService::run() {
    prepareServerSocket();
    
    fd_set read_fds;
    int max_fd = this->server_socket_->fd();
    
    auto http_handler_it = protocol_handlers_.find(ServiceType::HTTP);
    if (http_handler_it == protocol_handlers_.end()) {
        Logger::getInstance().log(LogLevel::Error, "No HTTP protocol handler registered.");
        return;
    }
    auto& http_handler = *(http_handler_it->second);
    
    std::vector<std::future<void>> connection_futures;
    ConnectionPool connection_pool;
    
    while (!this->stop_flag_) {
        FD_ZERO(&read_fds);
        FD_SET(this->server_socket_->fd(), &read_fds);
        
        timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0;
        
        int nfds = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);
        
        if (nfds < 0) {
            Logger::getInstance().log(LogLevel::Error, "select() error: " + std::string(strerror(errno)));
            break;
        }
        
        if (nfds == 0) {
            continue;
        }
        if (nfds > 0 && FD_ISSET(this->server_socket_->fd(), &read_fds)) {
            processNewConnection(connection_pool, http_handler, connection_futures);
        }
        
        cleanUpConnectionFutures(connection_futures);
    }
    for (auto& future : connection_futures) {
        future.wait();
    }
}


void HttpService::prepareServerSocket() {
    try {
        this->server_socket_->bind("127.0.0.1", 8080);
        this->server_socket_->listen();
    } catch (const std::exception& ex) {
        Logger::getInstance().log(LogLevel::Error, "Failed to bind and listen: " + std::string(ex.what()));
    }
}

void HttpService::processNewConnection(ConnectionPool &connection_pool, ProtocolHandler &http_handler, std::vector<std::future<void>> &connection_futures) {
    sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int client_fd = accept(this->server_socket_->fd(), (struct sockaddr *)&address, &addrlen);
    
    if (client_fd >= 0) {
        Logger::getInstance().log(LogLevel::Info, "Incoming connection");
        
        try {
            auto connection_ptr = std::make_shared<Connection>(client_fd);
            connection_pool.addConnection(connection_ptr);
            // Handle the request asynchronously
            connection_futures.push_back(std::async(std::launch::async, [&http_handler, &connection_pool]() {
                auto connection = connection_pool.getConnection();
                try {
                    http_handler.handleRequest(*connection);
                } catch (const std::exception& ex) {
                    connection->socket().close();
                }                
            }));
        } catch (const std::exception& ex) {
            Logger::getInstance().log(LogLevel::Error, "Failed to create connection: " + std::string(ex.what()));
        }
    } else {
        Logger::getInstance().log(LogLevel::Error, "accept() error: " + std::string(strerror(errno)));
    }
}

void HttpService::cleanUpConnectionFutures(std::vector<std::future<void>> &connection_futures) {
    connection_futures.erase(
                             std::remove_if(connection_futures.begin(), connection_futures.end(),
                                            [](const std::future<void>& f) {
                                                return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
                                            }),
                             connection_futures.end());
}

void HttpService::registerProtocolHandler(const std::string& protocol, std::unique_ptr<ProtocolHandler> handler) {
    protocol_handlers_.emplace(protocol, std::move(handler));
}

void HttpService::stop() {    
    this->stop_flag_ = true;
}
