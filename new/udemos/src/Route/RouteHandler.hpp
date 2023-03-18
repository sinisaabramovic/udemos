//
//  RouteHandler.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#ifndef RouteHandler_hpp
#define RouteHandler_hpp

#include <stdio.h>
#include "RouteMatcher.hpp"

class HttpResponse;
class HttpRequest;

class RouteHandler {
public:
    RouteHandler(const std::string& method, const std::string& path);
    virtual ~RouteHandler() = default;

    const std::string& getMethod() const;
    const std::string& getPath() const;

    virtual std::shared_ptr<HttpResponse> handleRequest(const HttpRequest& request) = 0;

protected:
    std::string method_;
    std::string path_;
};

#endif /* RouteHandler_hpp */
