#ifndef UD_ROUTER_H
#define UD_ROUTER_H

#include <iostream>
#include <memory>
#include <vector>

#include "reactive/udFlux.hpp"

class udConnection;
class udHttpRequest;
class udRoute;

class udRouter
{
private:
    std::vector<std::shared_ptr<udRoute>> routes_;
    std::shared_ptr<udRequestStream> request_stream_;

public:
    udRouter();
    void AddRoute(std::shared_ptr<udRoute> route);
    void HandleRequest(std::shared_ptr<udHttpRequest> request, std::shared_ptr<udConnection> connection);
    void attach_request_stream(std::shared_ptr<udRequestStream> request_stream);    
};

#endif // UD_ROUTER_H
