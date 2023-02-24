#ifndef ud_http_route_hpp
#define ud_http_route_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../controller/ud_http_controller.hpp"
#include "../view/ud_http_view.hpp"
#include "../models/request/ud_http_request.hpp"

class ud_http_route
{
private:
    std::string m_path;
    std::string m_method;
    std::shared_ptr<ud_http_controller> m_controller;

public:
    ud_http_route(std::string path, std::string method, std::shared_ptr<ud_http_controller> controller) : m_path(path), m_method(method), m_controller(std::move(controller)) {}

    std::string handle_request(const ud_http_request &request)
    {
        return m_controller->handle_request(request);
    }

    std::string get_path() const
    {
        return m_path;
    }

    std::string get_method() const
    {
        return m_method;
    }

    bool is_accessable(const ud_http_request &request)
    {
        return get_path() == request.get_path() && get_method() == request.get_method();
    }
};

#endif