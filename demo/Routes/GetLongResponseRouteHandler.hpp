//
//  GetLongResponseRouteHandler.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 24.03.2023..
//

#ifndef GetLongResponseRouteHandler_hpp
#define GetLongResponseRouteHandler_hpp

#include <stdio.h>
#include "../../src/Route/RouteHandler.hpp"

class HttpRequest;
class HttpResponse;

class GetLongResponseRouteHandler : public RouteHandler {
public:
    GetLongResponseRouteHandler(const std::string& method, const std::string& path);
    virtual ~GetLongResponseRouteHandler() = default;

    virtual std::shared_ptr<HttpResponse> handleRequest(const HttpRequest& request) override;
};

#endif /* GetLongResponseRouteHandler_hpp */
