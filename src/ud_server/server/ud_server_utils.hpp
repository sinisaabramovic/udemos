#ifndef ud_server_utils_hpp
#define ud_server_utils_hpp

#include <thread>
#include <chrono>
#include <atomic>
#include <random>

#include "ud_server_data.hpp"

#include "../common/ud_thread_manager.hpp"
#include "../acceptor/ud_http_acceptor.hpp"
#include "../connection/ud_http_connection.hpp"
#include "../router/ud_http_router.hpp"
#include "../common/ud_http_thread_pool.hpp"

class ud_server_utils
{
public:
    static void listen(ud_server_data &data)
    {
        using ud_result_type = ud_result<ud_result_success, ud_result_failure>;
        int client_socket_fd;
        bool active = true;

        while (!data.get_stoped())
        {
            if (!active)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(data.get_sleep_times()(data.get_rng())));
            }

            if ((client_socket_fd = data.get_acceptor()->accept_connection()) <= 0)
            {
                active = false;
                auto result = ud_result_type{ud_result_success{"listen accept rejected"}};
                close(client_socket_fd);
                continue;
            }
            active = true;

            std::shared_ptr<ud_http_connection<ud_http_router>> client_connection =
                std::make_shared<ud_http_connection<ud_http_router>>(client_socket_fd, data.get_router());
            data.get_thread_pool()->enqueue(&ud_http_connection<ud_http_router>::start, client_connection);
        }
    }
};

#endif
