#include "../server/udHttpRequest.hpp"
#include "../server/udHttpResponse.hpp"
#include "HelloWorldRoute.hpp"

HelloWorldRoute::HelloWorldRoute() : udRoute("GET", "/hello") {}

std::shared_ptr<udHttpResponse> HelloWorldRoute::Handle(std::shared_ptr<udHttpRequest> request)
{
    auto response = std::make_shared<udHttpResponse>(200);
    response->SetHeader("Content-Type", "text/plain");
    response->SetBody("Hello, world!\n");
    return response;
}
