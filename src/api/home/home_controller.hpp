#ifndef home_controller_hpp
#define home_controller_hpp

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "../../ud_server/controller/ud_http_controller.hpp"
#include "../../ud_server/view/ud_http_view.hpp"
#include "home_view.hpp"

class home_controller : public ud_http_controller
{
public:
    home_controller(std::shared_ptr<ud_http_view> view) : ud_http_controller(std::move(view)) {}

    std::string handle_request(const ud_http_request &request) override
    {
        return this->view->render(request);
    }
};

#endif