#ifndef HTTP_CONTROLLER_HPP
#define HTTP_CONTROLLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../view/ud_http_view.hpp"

class HttpRequest;

class ud_http_controller
{
protected:
    std::shared_ptr<ud_http_view> view;

public:
    ud_http_controller(std::shared_ptr<ud_http_view> view) : view(std::move(view)){};
    ~ud_http_controller(){};

    virtual std::string handle_request(const ud_http_request &request) = 0;
};

#endif