#ifndef home_view_hpp
#define home_view_hpp

#include <chrono>
#include <thread>
#include <iostream>
#include <ostream>
#include <string>

#include "../../ud_server/models/response/ud_http_response_generator_factory.hpp"
#include "../../ud_server/models/response/util/ud_http_response_utils.hpp"
#include "../../ud_server/view/ud_http_view.hpp"
#include "../../ud_server/common/ud_http_time.hpp"

class home_view : public ud_http_view
{
public:
    home_view() {}
    ~home_view() {}

    std::string render(const ud_http_request &request) override
    {
        // FOR DEMO ONLY - simulates slow response
        // std::this_thread::sleep_for(std::chrono::seconds(5));
        // const std::string compressed_payload = HttpResponseUtils::CompressPayload(request.get_body());
        std::string current_time = ud_http_time::current_date_time();
        std::string payload = R"(RUNNER)"
                              R"( followed by not a newline: \n)"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " which is then followed by a non-raw literal that's concatenated \n with"
                              " an embedded non-raw newline";

        std::string logo = R"(UDEMOS)"
                        "  _    _ _____  ______ __  __  ____   _____  \n"
                        " | |  | |  __ \\|  ____|  \\/  |/ __ \\ / ____| \n" 
                        " | |  | | |  | | |__  | \\  / | |  | | (___   \n" 
                        " | |  | | |  | |  __| | |\\/| | |  | |\\___ \\  \n" 
                        " | |__| | |__| | |____| |  | | |__| |____) | \n" 
                        "  \\____/|_____/|______|_|  |_|\\____/|_____/  \n";

        const std::string payload_result = "{ ""payload:"" ""pong"", ""at"":  """ + current_time + """, ""data: "" "" " + payload + " "" logo: " + logo + "}";        
        auto responseGenerator = ud_http_response_generator_factory::create_generator();
        return responseGenerator->create_generator(ud_http_status_codes::OK, "application/json", payload_result);
    }
};

#endif