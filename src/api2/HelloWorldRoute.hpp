#ifndef HELLO_WORLD_ROUTE_HPP
#define HELLO_WORLD_ROUTE_HPP

#include "../server/udRoute.hpp"

class udHttpRequest;
class udHttpResponse;

class HelloWorldRoute : public udRoute
{
public:
    HelloWorldRoute();
    std::shared_ptr<udHttpResponse> Handle(std::shared_ptr<udHttpRequest> request);
};

#endif