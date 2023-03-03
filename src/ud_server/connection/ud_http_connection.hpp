#ifndef ud_http_connection_hpp
#define ud_http_connection_hpp

#include <memory>
#include <string>
#include <sys/socket.h>

#include "../common/ud_http_defines.h"
#include "../router/ud_http_router.hpp"

#include "../socket/ud_http_socket.hpp"
#include "../common/result/ud_common_result.hpp"

template <typename T>
class ud_http_connection
{
private:
    std::shared_ptr<ud_http_socket> m_socket;
    std::shared_ptr<T> m_router;

    result<bool, ud_error> send_response_to_client(const std::string &response);
    result<std::shared_ptr<std::string>, ud_error> read_request_from_client();
    result<bool, ud_error> handle_client_request(fd_set *readfds);
    int32_t wait_for_activity(int32_t max_sd, fd_set *readfds);

public:
    ud_http_connection(int32_t socket, const std::shared_ptr<T> &router) : m_socket(std::make_shared<ud_http_socket>(socket)),
                                                                           m_router(std::move(router)){};
    ~ud_http_connection() {}

    void start();
};

template <typename T>
void ud_http_connection<T>::start()
{
    while (true)
    {
        if (this->m_socket->get_socket() <= 0)
        {
            break;
        }

        fd_set readfds;
        int max_sd = this->m_socket->get_socket();
        FD_ZERO(&readfds);
        FD_SET(max_sd, &readfds);

        int activity = wait_for_activity(max_sd, &readfds);

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

        auto client_request_result = handle_client_request(&readfds);
        if (!client_request_result.is_ok())
        {
            // An error occurred while handling the client request
            break;
        }
    }

    this->m_socket->close_socket();
}

template <typename T>
int32_t ud_http_connection<T>::wait_for_activity(int32_t max_sd, fd_set *readfds)
{
    // Wait for activity on any of the file descriptors
    return select(max_sd + 1, readfds, nullptr, nullptr, nullptr);
}

template <typename T>
result<bool, ud_error> ud_http_connection<T>::handle_client_request(fd_set *readfds)
{
    // If the client socket has data to read
    if (!FD_ISSET(this->m_socket->get_socket(), readfds))
    {
        return ud_error{"The socket was closed, exit the thread"};
    }

    auto request = read_request_from_client();
    if (!request.is_ok()) {
        return ud_error{"An error occurred while reading the request"};
    } 

    std::string response = m_router->process_request(request.value());
    if (response.empty())
    {
        return ud_error{"An error occurred while reading the request"};
    }

    auto handle_response = send_response_to_client(response);
    if (!handle_response.is_ok())
    {
        return ud_error{"An error occurred while sending the response"};
    }

    return true;
}

template <typename T>
result<std::shared_ptr<std::string>, ud_error> ud_http_connection<T>::read_request_from_client()
{
    std::shared_ptr<std::string> request = std::make_shared<std::string>();
    char buffer[MAX_BUFFER_SIZE];

    while (true)
    {
        int bytes_read = read(this->m_socket->get_socket(), buffer, MAX_BUFFER_SIZE);
        if (bytes_read < 0)
        {
            this->m_socket->close_socket();
            request.reset();
            return ud_error{"error read request"};
        }
        else if (bytes_read == 0)
        {
            this->m_socket->close_socket();
            request.reset();
            return ud_error{"error client is disconnected"};
        }

        request->append(buffer, bytes_read);

        if (bytes_read < MAX_BUFFER_SIZE)
        {
            // We've read all the available data, so exit the loop
            break;
        }
    }

    return request;
}

template <typename T>
result<bool, ud_error> ud_http_connection<T>::send_response_to_client(const std::string &response)
{
    int bytes_sent = send(this->m_socket->get_socket(), response.c_str(), response.size(), 0);
    if (bytes_sent < 0)
    {
        this->m_socket->close_socket();
        return ud_error{"error in send"};
    }
    else if (bytes_sent == 0)
    {
        this->m_socket->close_socket();
        return ud_error{"client in disconnected"};
    }

    return true;
}

#endif