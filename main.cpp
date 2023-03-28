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

#include "src/Core/Config/Configuration.hpp"
#include "src/Protocol/HttpProtocolHandler.h"
#include "src/Route/RouteHandlerFactory.hpp"

#include "demo/Routes/GetRouteHandler.hpp"
#include "demo/Routes/Secrets/GetSecretRouteHandler.hpp"
#include "demo/Routes/GetLongResponseRouteHandler.hpp"
#include "demo/Routes/SimpleSecrets/GetSimpleSecretRouteHandler.hpp"

#include "src/Protocol/HttpModels/HttpMethod.hpp"
#include "src/Core/Logger/Logger.hpp"
#include "src/Services/ServiceType.hpp"
#include "src/Services/HttpService.hpp"

int main(int argc, char **argv)
{

    Logger::getInstance().log(LogLevel::Info, "Server started");

    RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetRouteHandler>(HttpMethod::POST, "/api/simple"));

    RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetSecretRouteHandler>(HttpMethod::POST, "/api/secret"));

    RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetSimpleSecretRouteHandler>(HttpMethod::POST, "/api/simplesecret"));

    RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetLongResponseRouteHandler>(HttpMethod::GET, "/api/long"));

    Configuration &config = Configuration::getInstance();
    config.set("host", "0.0.0.0");
    config.set("port", 8080);

    std::shared_ptr<HttpService> httpService = std::make_shared<HttpService>(config);
    httpService->registerProtocolHandler(ServiceType::HTTP, std::make_unique<HttpProtocolHandler>());

    // Start the service in a separate thread
    std::thread service_thread([&httpService]()
                               { httpService->run(); });

    Logger::getInstance().log(LogLevel::Info, "Server is running. Press 'q' and Enter to stop the server.");

    // Wait for user input in the main thread
    char input;
    do
    {
        std::cin >> input;
    } while (input != 'q');

    Logger::getInstance().log(LogLevel::Info, "Stopping the server...");

    // Stop the service and wait for the service thread to finish
    httpService->stop();
    service_thread.join();

    Logger::getInstance().log(LogLevel::Info, "Server stopped");

    return 0;
}
