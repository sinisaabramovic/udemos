#include <iostream>

// #include "src/Server/HttpServer.hpp"
// #include "src/API/Home/HomeController.hpp"
// #include "src/Route/HttpRoute.hpp"
// #include "src/Router/HttpRouter.hpp"

// #include "src/Logger/HttpLogType.hpp"
// #include "src/Logger/HttpLogger.h"
// #include "src/Logger/HttpLoggerMediator.h"
// #include "src/Logger/Logs/HttpLogCommon.hpp"

#include "src/ud_server/ud_http.hpp"

int main()
{

    // auto mediator = std::make_shared<HttpLoggerMediator>();
    // auto logger = std::make_shared<HttpLogger>(mediator);

    // mediator->add_logger(logger);

    // logger->log(HttpLogLevel::INFO, "       This is an info message");
    // logger->log(HttpLogLevel::WARNING, "    This is an warning message");
    // logger->log(HttpLogLevel::ERROR, "      This is an error message");
    // logger->log(HttpLogLevel::FATAL_ERROR, "This is an fatal error message");

    // std::shared_ptr<HomeView> homeView = std::make_shared<HomeView>();
    // std::shared_ptr<HomeController> home_controller = std::make_shared<HomeController>(homeView);
    // std::shared_ptr<HttpRoute> home_route = std::make_shared<HttpRoute>("/", "GET", home_controller);

    // std::shared_ptr<HttpRouter> router = std::make_shared<HttpRouter>();
    // router->add_route(home_route);

    // std::shared_ptr<HttpServer<HttpRouter>> server = std::make_shared<HttpServer<HttpRouter>>(
    //     8080,
    //     router,
    //     "/Users/sinisaabramovic/projects/udemos/bin/cert.pem",
    //     "/Users/sinisaabramovic/projects/udemos/bin/key.pem",
    //     true);
    // server->start();

    std::shared_ptr<ud_http> server = std::make_shared<ud_http>();
    server->start_listen(8080, "localhost", [](const ud_result<ud_result_success, ud_result_failure> &server_result)
    {        
        if (server_result.is_success()) {
            std::cout << server_result.get_value().get_description() << std::endl;
        } else {
            std::cout << server_result.get_error().get_description() << std::endl;
        } 
    });

// TEST - for test only 
    while (server->is_running())
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        {
            server->pause_listen(true);
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
        {
            server->pause_listen(false);
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
        {
            server->stop_listen();
        }
    }

    std::cout << "Program ended! \n";
    return 0;
}
