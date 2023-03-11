#ifndef ud_http_connection_hpp
#define ud_http_connection_hpp

#include <memory>
#include <string>
#include <sys/socket.h>

#include "../common/ud_http_defines.h"
#include "../router/ud_http_router.hpp"

#include "../socket/ud_http_socket.hpp"
#include "../common/result/ud_common_result.hpp"
#include "ud_http_connection_data.hpp"
#include "ud_http_connection_util.hpp"

template <typename T>
class ud_http_connection
{
public:
    ud_http_connection(const ud_http_connection_data<T> &data) : m_data(data){};
    ud_http_connection(int32_t socket, const std::shared_ptr<T> &router)
        : m_data(socket, router){};

    ~ud_http_connection() {}

    void start();

private:
    ud_http_connection_data<T> m_data;
};

template <typename T>
void ud_http_connection<T>::start()
{
    while (true)
    {
        if (m_data.m_socket->get_socket() <= 0)
        {
            break;
        }

        fd_set readfds;
        int max_sd = m_data.m_socket->get_socket();
        FD_ZERO(&readfds);
        FD_SET(max_sd, &readfds);

        int activity = ud_http_connection_util<T>::wait_for_activity(max_sd, &readfds);

        if (activity < 0)
        {
            perror("select error");
            break;
        }

        if (activity == 0)
        {
            // The timeout has expired, do nothing
            continue;
        }

        auto client_request_result = ud_http_connection_util<T>::handle_client_request(&readfds, m_data);
        if (!client_request_result.is_ok())
        {
            // An error occurred while handling the client request
            break;
        }
    }

    m_data.m_socket->close_socket();
}

#endif