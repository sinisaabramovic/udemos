//
//  GetBaseRouteHandler.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#include <stdio.h>
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "GetRouteHandler.hpp"

GetRouteHandler::GetRouteHandler(const std::string& method, const std::string& path) : RouteHandler(method, path) {}

std::shared_ptr<HttpResponse> GetRouteHandler::handleRequest(const HttpRequest& request) {
    auto response = std::make_shared<HttpResponse>();
    response->setStatusCode(HttpResponseStatus::OK);
    response->addHeader("Content-Type", "text/plain");
    response->setBody("Hello, World!");
    
    return response;
}
