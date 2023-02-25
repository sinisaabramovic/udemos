#include <iostream>
#include <cstdlib>

#include "src/ud_server/router/ud_http_route.hpp"
#include "src/ud_server/router/ud_http_router.hpp"
#include "src/ud_server/router/ud_http_route.hpp"
#include "src/ud_server/ud_http.hpp"

#include "src/api/home/home_controller.hpp"
#include "src/api/home/home_view.hpp"

int main()
{

    // auto mediator = std::make_shared<HttpLoggerMediator>();
    // auto logger = std::make_shared<HttpLogger>(mediator);

    // mediator->add_logger(logger);

    // logger->log(HttpLogLevel::INFO, "       This is an info message");
    // logger->log(HttpLogLevel::WARNING, "    This is an warning message");
    // logger->log(HttpLogLevel::ERROR, "      This is an error message");
    // logger->log(HttpLogLevel::FATAL_ERROR, "This is an fatal error message");

    std::shared_ptr<home_view> home_v = std::make_shared<home_view>();
    std::shared_ptr<home_controller> home_ctrl = std::make_shared<home_controller>(home_v);
    std::shared_ptr<ud_http_route> home_route = std::make_shared<ud_http_route>("/", "GET", home_ctrl);

    std::shared_ptr<ud_http_router> router = std::make_shared<ud_http_router>();
    router->add_route(home_route);

    std::shared_ptr<ud_http> http_server = std::make_shared<ud_http>();
    http_server->start_listen(
        8080, "localhost",
        router, [](const ud_result<ud_result_success, ud_result_failure> &status_info)
        {        
        if (status_info.is_success()) {
            std::cout << status_info.get_value().get_description() << std::endl;
        } else {
            std::cout << status_info.get_error().get_description() << std::endl;
        } });

    // TEST - for test only
    // while (server->is_running())
    // {
    //     std::this_thread::sleep_for(std::chrono::seconds(3));
    //     {
    //         server->pause_listen(true);
    //     }

    //     std::this_thread::sleep_for(std::chrono::seconds(3));
    //     {
    //         server->pause_listen(false);
    //     }

    //     std::this_thread::sleep_for(std::chrono::seconds(10));
    //     {
    //         server->stop_listen();
    //     }
    // }

    std::cout << "Enter [q] to stop the server" << std::endl;
    std::string command;
    while (std::cin >> command, command != "q")
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "'QUIT' command entered. Stopping the web server.."
              << std::endl;

    http_server->stop_listen();

    std::cout << "Server stopped" << std::endl;

    return 0;
}
