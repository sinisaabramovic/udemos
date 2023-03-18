//
//  main.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifdef DEBUG
#include <gtest/gtest.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "Configuration.hpp"
#include "ServiceFactory.hpp"
#include "HttpProtocolHandler.h"
#include "RouteHandlerFactory.hpp"
#include "GetRouteHandler.hpp"

int main(int argc, char **argv) {
    //#ifdef DEBUG
    //    ::testing::InitGoogleTest(&argc, argv);
    //    return RUN_ALL_TESTS();
    //#endif
    //
    
    std::cout << "Server started" << std::endl;
    
    RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetRouteHandler>("GET", "/api"));
    
    Configuration& config = Configuration::getInstance();
    config.set("host", "127.0.0.1");
    config.set("port", 8080);
    
    BaseService service(config);
    service.registerProtocolHandler("http", std::make_unique<HttpProtocolHandler>());
    
    service.run();
    
    std::cout << "Server is running. Press 'q' and Enter to stop the server." << std::endl;
    
    char input;
    do {
        std::cin >> input;
    } while (input != 'q');
    
    std::cout << "Stopping the server..." << std::endl;
//    service.stop();
    std::cout << "Server stopped." << std::endl;
    
    return 0;
}
