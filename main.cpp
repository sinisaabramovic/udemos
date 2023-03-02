#include <iostream>
#include <cstdlib>

#include "src/ud_server/router/ud_http_route.hpp"
#include "src/ud_server/router/ud_http_router.hpp"
#include "src/ud_server/router/ud_http_route.hpp"
#include "src/ud_server/ud_http.hpp"

#include "src/api/home/home_controller.hpp"
#include "src/api/home/home_view.hpp"

#include "src/security/encrypt_aes.hpp"

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

    std::shared_ptr<ud_http_router> router = std::make_shared<ud_http_router>();
    router->add_route(home_route);
    router->add_route(user_route);

    try
    {
        std::shared_ptr<ud_http> http_server = std::make_shared<ud_http>();

        http_server->start_listen(router, [](const ud_result<ud_result_success, ud_result_failure> &status_info)
                                  {        
        if (status_info.is_success()) {
            std::cout << status_info.get_value().get_description() << std::endl;
        } else {
            std::cout << status_info.get_error().get_description() << std::endl;
        } });

        std::cout << "Enter [q] to stop the server" << std::endl;
        std::string command;
        while (std::cin >> command, command != "q")
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "'QUIT' command entered. Stopping the web server.."
                  << std::endl;

        http_server->stop_listen();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "Server stopped" << std::endl;

    return 0;
}
