//
//  ServiceFactoryTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <stdio.h>
#include <gtest/gtest.h>
#include "Configuration.hpp"
#include "ServiceFactory.hpp"
#include "HttpProtocolHandler.h"

//TEST(ServerFactoryTests, CreateHttpServer) {
//    // Set up configuration object
//    
//    Configuration& config = Configuration::getInstance();
//    
//    // Set environment variables
//    setenv("port", "8080", 1);
//    setenv("host", "localhost", 1);
//    
//    // Load the configuration from the environment variables
//    config.loadFromEnvironment();
//    
//    // Create server factory
//    auto service = ServiceFactory::createService(config);
//    
//    // Register HTTP protocol handler
//    service->registerProtocolHandler("http", std::make_unique<HttpProtocolHandler>());
//    
//    // Check that server was created with correct protocol handler
//    EXPECT_TRUE(dynamic_cast<HttpProtocolHandler*>(service->getProtocolHandler("http")->get()) != nullptr);
//    
//}
