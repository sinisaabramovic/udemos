//
//  GetBaseRouteHandler.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#ifndef GetBaseRouteHandler_hpp
#define GetBaseRouteHandler_hpp

#include <stdio.h>
#include "../../src/Route/RouteHandler.hpp"

class HttpRequest;
class HttpResponse;

class GetRouteHandler : public RouteHandler {
public:
    GetRouteHandler(const std::string& method, const std::string& path);
    virtual ~GetRouteHandler() = default;

    virtual std::shared_ptr<HttpResponse> handleRequest(const HttpRequest& request) override;
};

#endif /* GetBaseRouteHandler_hpp */
