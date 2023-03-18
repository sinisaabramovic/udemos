//
//  RouteHandlerFactory.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#ifndef RouteHandlerFactory_hpp
#define RouteHandlerFactory_hpp

#include <stdio.h>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include "RouteHandler.hpp"

class RouteHandlerFactory {
public:
    // Delete copy constructor and assignment operator
    RouteHandlerFactory(const RouteHandlerFactory&) = delete;
    RouteHandlerFactory& operator=(const RouteHandlerFactory&) = delete;

    // Get the singleton instance
    static RouteHandlerFactory& getInstance() {
        static RouteHandlerFactory instance;
        return instance;
    }

    std::shared_ptr<RouteHandler> getHandler(const std::string& method, const std::string& path) {
        auto key = method + ":" + path;
        auto it = handlers.find(key);
        if (it != handlers.end()) {
            return it->second;
        }
        return nullptr;
    }

    void registerHandler(const std::shared_ptr<RouteHandler>& handler) {
        auto key = handler->getMethod() + ":" + handler->getPath();
        handlers[key] = handler;
    }

private:
    // Make the constructor private
    RouteHandlerFactory() {}

    std::unordered_map<std::string, std::shared_ptr<RouteHandler>> handlers;
};


#endif /* RouteHandlerFactory_hpp */
