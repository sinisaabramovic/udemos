#ifndef HTTP_HOME_VIEW_HPP
#define HTTP_HOME_VIEW_HPP

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "../../Common/Response/HttpResponseGeneratorFactory.hpp"
#include "../../Common/HttpTime.hpp"
#include "../../Common/Response/Util/HttpResponseUtils.hpp"

class HomeView : public HttpView
{
public:
    HomeView() {}
    ~HomeView() {}

    std::string render(const HttpRequest &request) override
    {
        // FOR DEMO ONLY - simulates slow response
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // const std::string compressed_payload = HttpResponseUtils::CompressPayload(request.get_body());
        const std::string payload = "HELLO!!!";
        auto responseGenerator = HttpResponseGeneratorFactory::CreateGenerator();
        return responseGenerator->GenerateResponse(HttpStatusCodes::OK, "text/plain", payload);
    }
};

#endif