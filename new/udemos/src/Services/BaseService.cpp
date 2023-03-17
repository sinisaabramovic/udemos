//
//  BaseService.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "BaseService.hpp"

BaseService::BaseService(const Configuration& config) : config_(config.getInstance()) {}
BaseService::~BaseService() {}

void BaseService::run() {
    // Initialize and configure the server based on the config_ object.
    // Set up connections and event handling using the event_loop_ object.

    // Call event_loop_.run() to start the event loop.
    event_loop_.run();
}

void BaseService::registerProtocolHandler(const std::string& protocol, std::unique_ptr<ProtocolHandler> handler) {
    protocol_handlers_.emplace(protocol, std::move(handler));
}

std::unique_ptr<ProtocolHandler> BaseService::getProtocolHandler(const std::string& protocol) {
    auto it = protocol_handlers_.find(protocol);
    if (it == protocol_handlers_.end()) {
        return nullptr;
    }
    return std::move(it->second);
}
