//
//  ServiceFactory.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "ServiceFactory.hpp"
#include "HttpProtocolHandler.h"
#include "ServiceType.hpp"

std::unique_ptr<HttpService> ServiceFactory::createService(const Configuration& config) {
    auto service = std::make_unique<HttpService>(config);

    service->registerProtocolHandler(ServiceType::HTTP, std::make_unique<HttpProtocolHandler>());

    // Register other protocol handlers here as needed, e.g., HTTPS, FTP, FTPS, RPC, and RPCS.

    return service;
}
