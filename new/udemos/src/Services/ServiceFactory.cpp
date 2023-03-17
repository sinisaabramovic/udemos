//
//  ServiceFactory.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "ServiceFactory.hpp"
#include "HttpProtocolHandler.h"

std::unique_ptr<BaseService> ServiceFactory::createService(const Configuration& config) {
    auto service = std::make_unique<BaseService>(config);

    // Register the HTTP protocol handler.
    service->registerProtocolHandler("http", std::make_unique<HttpProtocolHandler>());

    // Register other protocol handlers here as needed, e.g., HTTPS, FTP, FTPS, RPC, and RPCS.

    return service;
}
