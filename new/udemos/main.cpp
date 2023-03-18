//
//  main.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <thread>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "Configuration.hpp"
#include "ServiceFactory.hpp"
#include "HttpProtocolHandler.h"
#include "RouteHandlerFactory.hpp"
#include "GetRouteHandler.hpp"
#include "HttpMethod.hpp"
#include "Logger.hpp"
#include "ServiceType.hpp"

int main(int argc, char **argv) {
    
    Logger::getInstance().log(LogLevel::Info, "Server started");
    
    RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetRouteHandler>(HttpMethod::GET, "/api"));
    
    Configuration& config = Configuration::getInstance();
    config.set("host", "127.0.0.1");
    config.set("port", 8080);
    
    HttpService service(config);
    service.registerProtocolHandler(ServiceType::HTTP, std::make_unique<HttpProtocolHandler>());
    
    // Start the service in a separate thread
    std::thread service_thread([&service]() {
        service.run();
    });
    
    Logger::getInstance().log(LogLevel::Info, "Server is running. Press 'q' and Enter to stop the server.");
    
    // Wait for user input in the main thread
    char input;
    do {
        std::cin >> input;
    } while (input != 'q');
    
    Logger::getInstance().log(LogLevel::Info, "Stopping the server...");
    
    // Stop the service and wait for the service thread to finish
    service.stop();
    service_thread.join();
    
    Logger::getInstance().log(LogLevel::Info, "Server stopped");
    
    return 0;
}
