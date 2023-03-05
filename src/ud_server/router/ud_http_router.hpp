#ifndef ud_http_router_hpp
#define ud_http_router_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <future>

#include "ud_http_route.hpp"
#include "../models/request/ud_http_request.hpp"
#include "../models/response/ud_http_response.hpp"
#include "../common/result/ud_common_result.hpp"

class ud_http_router
{
private:
    std::map<std::string, std::shared_ptr<ud_http_route>> m_routes;

public:
    void add_route(const std::shared_ptr<ud_http_route> &route)
    {
        m_routes[route->get_path()] = route;
    }

    std::string process_request(const std::shared_ptr<std::string> &request)
    {
        if (request.get() == nullptr)
        {
            auto response = ud_http_response(ud_response_status_code::NOT_FOUND, "application/json", "Bad request");
            return response.to_string();
        }
        ud_http_request parsed_request{request->data()};

        auto route_it = m_routes.find(parsed_request.get_path());
        if (route_it != m_routes.end())
        {
            std::shared_ptr<ud_http_route> route = route_it->second;
            if (route->is_accessable(parsed_request))
            {
                return route->handle_request(parsed_request);
            }
        }

        auto response = ud_http_response(ud_response_status_code::NOT_FOUND, "application/json", "Bad request");
        return response.to_string();
    }
};

#endif