//
//  GetSimpleSecretRouteHandler.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 28.03.2023..
//

#ifndef GetSimpleSecretRouteHandler_hpp
#define GetSimpleSecretRouteHandler_hpp

#include <stdio.h>
#include "../../../src/Route/RouteHandler.hpp"

class HttpRequest;
class HttpResponse;

class GetSimpleSecretRouteHandler : public RouteHandler {
public:
    GetSimpleSecretRouteHandler(const std::string& method, const std::string& path);
    virtual ~GetSimpleSecretRouteHandler() = default;

    virtual std::shared_ptr<HttpResponse> handleRequest(const HttpRequest& request) override;
};

#endif /* GetSimpleSecretRouteHandler_hpp */
