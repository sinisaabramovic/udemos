#ifndef home_view_hpp
#define home_view_hpp

#include <chrono>
#include <thread>
#include <iostream>
#include <ostream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "../../ud_server/models/response/ud_http_response.hpp"
#include "../../ud_server/view/ud_http_view.hpp"
#include "../../ud_server/common/ud_http_time.hpp"

#include "models/person.hpp"
#include "home_view_static_index_html.hpp"

#include "../../ud_server/balancer/ud_server_balancer.hpp"

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
             { return get_hello(); }},
            {"/user", [this](const ud_http_request &req)
             { return get_user(req); }},
            {"/time", [this](const ud_http_request &req)
             { return get_current_time(); }},
            {"/html", [this](const ud_http_request &req)
             { return get_html(); }},
            {"/performance", [this](const ud_http_request &req)
             { return get_performance(); }},
        };

        auto it = handlers.find(request.get_path());
        if (it != handlers.end())
        {
            return it->second(request);
        }
        else
        {
            auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", "Bad request");
            return response.to_string();
        }
    }

private:
    std::string get_user(const ud_http_request &request) const
    {
        if (request.get_body().empty())
        {
            auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", "Bad request");
            return response.to_string();
        }

        Document document;
        if (document.Parse(request.get_body().c_str()).HasParseError())
        {
            std::cout << "JSON parse error: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
            auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", "Bad request");
            return response.to_string();
        }

        document.Parse(request.get_body().c_str());

        person person_{};
        bool isParsed = person_.fromJson(document);
        if (!isParsed)
        {
            auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", "Bad request");
            return response.to_string();
        }

        person_.set_name("Jurica Pesic");
        person_.set_address("Hruskovica 39");
        person_.set_age(person_.get_age() + 1);

        auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", person_.toJSON());
        return response.to_string();
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
        auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", payload_result);
        return response.to_string();
    }

    std::string get_current_time() const
    {
        std::string current_time = ud_http_time::current_date_time();
        auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", current_time);
        return response.to_string();
    }

    std::string get_hello() const
    {
        auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", "hello");
        return response.to_string();
    }

    std::string get_html() const
    {
        home_view_static_index_html html_reader;
        auto html = html_reader.from_static_value();

        auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "text/plain", html);
        return response.to_string();
    }

    std::string get_performance() const
    {
        ud_server_balancer balancer;

        std::ostringstream jsonResponse;
        jsonResponse << "{";
        jsonResponse << "\"cpu\": \""
             << balancer.get_cpu_usage_description()
             << "\", ";
        jsonResponse << "\"memory\": " << balancer.get_memory_usage_description() << ", ";
        jsonResponse << "\"disk\": " << balancer.get_disk_usage_description() << true;
        jsonResponse << "}";

        auto response = ud_http_response(ud_response_status_code::BAD_REQUEST, "application/json", jsonResponse.str());
        return response.to_string();
    }
};

#endif