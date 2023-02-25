#ifndef home_view_hpp
#define home_view_hpp

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "../../ud_server/models/response/ud_http_response_generator_factory.hpp"
#include "../../ud_server/models/response/util/ud_http_response_utils.hpp"
#include "../../ud_server/view/ud_http_view.hpp"

class home_view : public ud_http_view
{
public:
    home_view() {}
    ~home_view() {}

    std::string render(const ud_http_request &request) override
    {
        // FOR DEMO ONLY - simulates slow response
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // const std::string compressed_payload = HttpResponseUtils::CompressPayload(request.get_body());
        const std::string payload = "HELLO!!!";
        auto responseGenerator = ud_http_response_generator_factory::create_generator();
        return responseGenerator->create_generator(ud_http_status_codes::OK, "text/plain", payload);
    }
};

#endif