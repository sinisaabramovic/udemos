#ifndef ud_http_connection_util_hpp
#define ud_http_connection_util_hpp

#include <memory>
#include <string>
#include <sys/socket.h>

#include "../common/ud_http_defines.h"
#include "../router/ud_http_router.hpp"

#include "../socket/ud_http_socket.hpp"
#include "../common/result/ud_common_result.hpp"

template <typename T>
class ud_http_connection_util
{
public:
    static result<bool, ud_error> send_response_to_client(const std::string &response, const ud_http_connection_data<T> &data)
    {
        int bytes_sent = send(data.m_socket->get_socket(), response.c_str(), response.size(), 0);
        if (bytes_sent < 0)
        {
            data.m_socket->close_socket();
            return ud_error{"error in send"};
        }
        else if (bytes_sent == 0)
        {
            data.m_socket->close_socket();
            return ud_error{"client in disconnected"};
        }

        return true;
    }

    static result<std::shared_ptr<std::string>, ud_error> read_request_from_client(const ud_http_connection_data<T> &data)
    {
        std::shared_ptr<std::string> request = std::make_shared<std::string>();
        char buffer[MAX_BUFFER_SIZE];
        int total_bytes_read = 0;

        while (true)
        {
            int bytes_read = read(data.m_socket->get_socket(), buffer, MAX_BUFFER_SIZE);
            if (bytes_read < 0)
            {
                data.m_socket->close_socket();
                request.reset();
                return ud_error{"error read request"};
            }
            else if (bytes_read == 0)
            {
                data.m_socket->close_socket();
                request.reset();
                return ud_error{"error client is disconnected"};
            }

            total_bytes_read += bytes_read;
            request->append(buffer, bytes_read);

            if (total_bytes_read >= MAX_REQUEST_SIZE)
            {
                return ud_error{"The request size is too large"};
            }

            if (bytes_read < MAX_BUFFER_SIZE)
            {
                // We've read all the available data, so exit the loop
                break;
            }
        }

        return request;
    }

    static result<bool, ud_error> handle_client_request(fd_set *readfds, const ud_http_connection_data<T> &data)
    {
        // If the client socket has data to read
        if (!FD_ISSET(data.m_socket->get_socket(), readfds))
        {
            return ud_error{"The socket was closed, exit the thread"};
        }

        auto request = read_request_from_client(data);
        if (!request.is_ok())
        {
            return ud_error{"An error occurred while reading the request"};
        }

        std::string response = data.m_router->process_request(request.value());
        if (response.empty())
        {
            return ud_error{"An error occurred while reading the request"};
        }

        auto handle_response = send_response_to_client(response, data);
        if (!handle_response.is_ok())
        {
            return ud_error{"An error occurred while sending the response"};
        }

        return true;
    }

    static int32_t wait_for_activity(int32_t max_sd, fd_set *readfds)
    {
        // Wait for activity on any of the file descriptors
        return select(max_sd + 1, readfds, nullptr, nullptr, nullptr);
    }
};

#endif