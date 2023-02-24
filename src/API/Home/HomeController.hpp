#ifndef HTTP_HOME_CONTROLLER_HPP
#define HTTP_HOME_CONTROLLER_HPP

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "../../Controllers/HttpController.hpp"
#include "../../View/HttpView.hpp"
#include "HomeView.hpp"

class HomeController : public HttpController
{
public:
    HomeController(std::shared_ptr<HttpView> view) : HttpController(std::move(view)) {}

    std::string handle_request(const HttpRequest &request) override
    {
        return this->view->render(request);
    }
};

#endif