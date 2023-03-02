#ifndef home_view_hpp
#define home_view_hpp

#include <chrono>
#include <thread>
#include <iostream>
#include <ostream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "../../ud_server/models/response/ud_http_response_generator_factory.hpp"
#include "../../ud_server/models/response/util/ud_http_response_utils.hpp"
#include "../../ud_server/view/ud_http_view.hpp"
#include "../../ud_server/common/ud_http_time.hpp"

#include "models/person.hpp"

using namespace rapidjson;

class home_view : public ud_http_view
{
public:
    home_view() {}
    ~home_view() {}

    std::string render(const ud_http_request &request) override
    {
        std::map<std::string, std::function<std::string(const ud_http_request &)>> handlers = {
            {"/", [this](const ud_http_request &req)
             { return dummy(); }},
            {"/user", [this](const ud_http_request &req)
             { return get_user(req); }},
            {"/time", [this](const ud_http_request &req)
             { return get_current_time(); }},
        };

        auto it = handlers.find(request.get_path());
        if (it != handlers.end())
        {
            return it->second(request);
        }
        else
        {
            auto responseGenerator = ud_http_response_generator_factory::create_generator();
            return responseGenerator->create_generator(ud_http_status_codes::BAD_REQUEST, "application/json", "Bad request");
        }
    }

private:
    std::string get_user(const ud_http_request &request) const
    {
        auto responseGenerator = ud_http_response_generator_factory::create_generator();
        if (request.get_body().empty())
        {
            return responseGenerator->create_generator(ud_http_status_codes::BAD_REQUEST, "application/json", "Invalid data");
        }

        Document document;
        if (document.Parse(request.get_body().c_str()).HasParseError())
        {
            std::cout << "JSON parse error: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
            return responseGenerator->create_generator(ud_http_status_codes::BAD_REQUEST, "application/json", "Invalid data");
        }

        document.Parse(request.get_body().c_str());

        person person_{};
        bool isParsed = person_.fromJson(document);
        if (!isParsed)
        {
            return responseGenerator->create_generator(ud_http_status_codes::BAD_REQUEST, "application/json", "Invalid data");
        }

        person_.set_name("Jurica Pesic");
        person_.set_address("Hruskovica 39");
        person_.set_age(person_.get_age() + 1);

        return responseGenerator->create_generator(ud_http_status_codes::OK, "application/json", person_.toJSON());
    }

    std::string dummy() const
    {
        // FOR DEMO ONLY - simulates slow response
        // std::this_thread::sleep_for(std::chrono::seconds(1));
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

        const std::string payload_result = "{ "
                                           "payload:"
                                           " "
                                           "pong"
                                           ", "
                                           "at"
                                           ":  "
                                           "" +
                                           current_time + ""
                                                          ", "
                                                          "data: "
                                                          " "
                                                          " " +
                                           payload + " "
                                                     " logo: " +
                                           logo + "}";
        auto responseGenerator = ud_http_response_generator_factory::create_generator();
        return responseGenerator->create_generator(ud_http_status_codes::OK, "application/json", payload_result);
    }

    std::string get_current_time() const
    {
        std::string current_time = ud_http_time::current_date_time();
        auto responseGenerator = ud_http_response_generator_factory::create_generator();
        return responseGenerator->create_generator(ud_http_status_codes::OK, "application/json", current_time);
    }
};

#endif