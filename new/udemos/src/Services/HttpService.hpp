//
//  BaseService.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef BaseService_hpp
#define BaseService_hpp

#include "ProtocolHandler.h"
#include "Configuration.hpp"
#include "EventLoop.h"
#include "Connection.hpp"
#include <unordered_map>
#include <memory>

class ThreadPool;

class HttpService {
public:
    HttpService(const Configuration& config);
    ~HttpService();
    void run();
    void stop();
    void registerProtocolHandler(const std::string& protocol, std::unique_ptr<ProtocolHandler> handler);
    
private:
    bool stop_flag_;
    Configuration& config_;
    EventLoop event_loop_;
    std::shared_ptr<Socket> server_socket_;
//    ThreadPool thread_pool(num_worker_threads);
    std::shared_ptr<ThreadPool> thread_pool_;
    std::unordered_map<std::string, std::unique_ptr<ProtocolHandler>> protocol_handlers_;
};

#endif /* BaseService_hpp */
