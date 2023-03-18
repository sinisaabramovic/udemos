//
//  BaseService.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "HttpService.hpp"
#include "ThreadPool.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/select.h>
#include "Logger.hpp"
#include "ServiceType.hpp"

HttpService::HttpService(const Configuration& config) : config_(config.getInstance())
{
    try {
        this->server_socket_ = std::make_shared<Socket>(this->event_loop_);
    } catch (const std::exception& ex) {
        Logger::getInstance().log(LogLevel::Error, "Failed to create BaseService: " + std::string(ex.what()));
    }
    this->stop_flag_ = false;
    this->thread_pool_ = std::make_shared<ThreadPool>(4);
}
HttpService::~HttpService() {}

void HttpService::run() {
    try {
        this->server_socket_->bind("127.0.0.1", 8080);
        this->server_socket_->listen();
    } catch (const std::exception& ex) {
        Logger::getInstance().log(LogLevel::Error, "Failed to bind and listen: " + std::string(ex.what()));
    }
    
    fd_set read_fds;
    int max_fd = this->server_socket_->fd();
    
    auto http_handler_it = protocol_handlers_.find(ServiceType::HTTP);
    if (http_handler_it == protocol_handlers_.end()) {
        Logger::getInstance().log(LogLevel::Error, "No HTTP protocol handler registered.");
        return;
    }
    auto& http_handler = *(http_handler_it->second);
    
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
        
        if (nfds > 0 && FD_ISSET(this->server_socket_->fd(), &read_fds)) {
            sockaddr_in address;
            socklen_t addrlen = sizeof(address);
            int client_fd = accept(this->server_socket_->fd(), (struct sockaddr *)&address, &addrlen);
            if (client_fd >= 0) {
                try {
                    auto connection_ptr = std::make_shared<Connection>(event_loop_, client_fd);
                    this->thread_pool_->enqueue([connection_ptr, &http_handler]() {
                        http_handler.handleRequest(*connection_ptr);
                    });
                } catch (const std::exception& ex) {
                    Logger::getInstance().log(LogLevel::Error, "Failed to create connection: " + std::string(ex.what()));
                }
            } else {
                Logger::getInstance().log(LogLevel::Error, "accept() error: " + std::string(strerror(errno)));
            }
        }
    }
}

void HttpService::registerProtocolHandler(const std::string& protocol, std::unique_ptr<ProtocolHandler> handler) {
    protocol_handlers_.emplace(protocol, std::move(handler));
}

void HttpService::stop() {    
    this->stop_flag_ = true;
}
