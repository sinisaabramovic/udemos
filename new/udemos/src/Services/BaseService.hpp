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

class BaseService {
public:
    BaseService(const Configuration& config);
    ~BaseService();
    void run();
    void registerProtocolHandler(const std::string& protocol, std::unique_ptr<ProtocolHandler> handler);
    ProtocolHandler* getProtocolHandler(const std::string& protocol);
    
private:
    Configuration& config_;
    EventLoop event_loop_;
    std::unordered_map<std::string, std::unique_ptr<ProtocolHandler>> protocol_handlers_;
};


#endif /* BaseService_hpp */
