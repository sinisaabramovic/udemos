#include <iostream>
#include <cstdlib>

#include "src/ud_server/router/ud_http_route.hpp"
#include "src/ud_server/router/ud_http_router.hpp"
#include "src/ud_server/router/ud_http_route.hpp"
#include "src/ud_server/server/ud_http_server.hpp"

#include "src/api/home/home_controller.hpp"
#include "src/api/home/home_view.hpp"

int main()
{

    std::cout << "  " << std::endl;
    std::cout << "  _    _ _____  ______ __  __  ____   _____     " << std::endl;
    std::cout << " | |  | |  __ \\|  ____|  \\/  |/ __ \\ / ____| " << std::endl;
    std::cout << " | |  | | |  | | |__  | \\  / | |  | | (___     " << std::endl;
    std::cout << " | |  | | |  | |  __| | |\\/| | |  | |\\___ \\  " << std::endl;
    std::cout << " | |__| | |__| | |____| |  | | |__| |____) |   " << std::endl;
    std::cout << "  \\____/|_____/|______|_|  |_|\\____/|_____/    " << std::endl;
    std::cout << "  " << std::endl;
    std::cout << "  " << std::endl;

    std::shared_ptr<home_view> home_v = std::make_shared<home_view>();
    std::shared_ptr<home_controller> home_ctrl = std::make_shared<home_controller>(home_v);
    std::shared_ptr<ud_http_route> home_route = std::make_shared<ud_http_route>("/", "GET", home_ctrl);
    std::shared_ptr<ud_http_route> user_route = std::make_shared<ud_http_route>("/user", "GET", home_ctrl);
    std::shared_ptr<ud_http_route> timer_route = std::make_shared<ud_http_route>("/time", "GET", home_ctrl);
    std::shared_ptr<ud_http_route> html_route = std::make_shared<ud_http_route>("/html", "GET", home_ctrl);
    std::shared_ptr<ud_http_route> performance_route = std::make_shared<ud_http_route>("/performance", "GET", home_ctrl);
    std::shared_ptr<ud_http_route> heavy_compute_route = std::make_shared<ud_http_route>("/strong", "GET", home_ctrl);

    std::shared_ptr<ud_http_router> router = std::make_shared<ud_http_router>();
    router->add_route(home_route);
    router->add_route(user_route);
    router->add_route(timer_route);
    router->add_route(html_route);
    router->add_route(performance_route);
    router->add_route(heavy_compute_route);    

    ud_http_server server(8080, "0.0.0.0");

    // Start listening for connections
    std::cout << "Server start listening for connections..." << std::endl;    
    server.start_listen(router);

    // Wait for user input to stop the server
    std::cout << "Press Enter to stop the server..." << std::endl;
    std::cin.ignore();

    // Stop the server
    server.stop_listen();

    return 0;
}
