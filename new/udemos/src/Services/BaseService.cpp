//
//  BaseService.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "BaseService.hpp"
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

BaseService::BaseService(const Configuration& config) : config_(config.getInstance()) {}
BaseService::~BaseService() {}

void BaseService::run() {
    Socket server_socket(event_loop_);
    server_socket.bind("127.0.0.1", 8080);
    server_socket.listen();
    
    // Set up connections and event handling
    size_t num_worker_threads = 256; // You can configure this based on your requirements
    ThreadPool thread_pool(num_worker_threads);
    
    fd_set read_fds;
    int max_fd = server_socket.fd();
    
    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket.fd(), &read_fds);
        
        timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0;
        
        int nfds = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);
        
        if (nfds < 0) {
            std::cerr << "select() error: " << strerror(errno) << std::endl;
            break;
        }
        
        if (nfds > 0 && FD_ISSET(server_socket.fd(), &read_fds)) {
            sockaddr_in address;
            socklen_t addrlen = sizeof(address);
            int client_fd = accept(server_socket.fd(), (struct sockaddr *)&address, &addrlen);
            if (client_fd >= 0) {
                std::shared_ptr<Connection> connection_ptr = std::make_shared<Connection>(event_loop_, client_fd);
                auto handler = getProtocolHandler("http");
                if (handler) {
                    thread_pool.enqueue([connection_ptr, handler]() {
                        handler->handleRequest(*connection_ptr);
                    });
                }
            } else {
                std::cerr << "accept() error: " << strerror(errno) << std::endl;
            }
        }
    }
}

void BaseService::registerProtocolHandler(const std::string& protocol, std::unique_ptr<ProtocolHandler> handler) {
    protocol_handlers_.emplace(protocol, std::move(handler));
}

ProtocolHandler* BaseService::getProtocolHandler(const std::string& protocol) {
    auto it = protocol_handlers_.find(protocol);
    if (it == protocol_handlers_.end()) {
        return nullptr;
    }
    return it->second.get();
}
