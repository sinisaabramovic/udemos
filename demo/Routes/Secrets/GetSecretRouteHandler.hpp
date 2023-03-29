//
//  GetSecretRouteHandler.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 21.03.2023..
//

#ifndef GetSecretRouteHandler_hpp
#define GetSecretRouteHandler_hpp

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include "../../../src/Route/RouteHandler.hpp"

class HttpRequest;
class HttpResponse;

class GetSecretRouteHandler : public RouteHandler {
public:
    GetSecretRouteHandler(const std::string& method, const std::string& path);
    virtual ~GetSecretRouteHandler() = default;

    virtual std::shared_ptr<HttpResponse> handleRequest(const HttpRequest& request) override;

private:
    std::string getJoke(int id) const;
};

#endif /* GetSecretRouteHandler_hpp */
