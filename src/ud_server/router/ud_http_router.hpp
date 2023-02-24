#ifndef ud_http_router_hpp
#define ud_http_router_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>

#include "../Common/HttpRequest.hpp"
#include "ud_http_route.hpp"
#include "../models/response/ud_http_response_generator_factory.hpp"

class ud_http_router
{
private:
    std::map<std::string, std::shared_ptr<ud_http_route>> m_routes;

public:
    void add_route(const std::shared_ptr<ud_http_route> &route)
    {
        m_routes[route->get_path()] = route;
    }

    std::string handle_request(const std::string &request)
    {
        ud_http_request parsed_request{request};

        auto route_it = m_routes.find(parsed_request.get_path());
        if (route_it != m_routes.end())
        {
            std::shared_ptr<ud_http_route> route = route_it->second;
            if (route->is_accessable(parsed_request))
            {
                return route->handle_request(parsed_request);
            }
        }

        auto response_generator = ud_http_response_generator_factory::create_generator();
        return response_generator->generate_response(ud_http_status_codes::NOT_FOUND, "application/json", "");
    }
};

#endif