#ifndef HTTP_CLIENT_CONNECTION_HPP
#define HTTP_CLIENT_CONNECTION_HPP

#include "../Defines/HttpDefines.h"
#include <memory>
#include <string>
#include <sys/socket.h>
#include "../Router/HttpRouter.hpp"

template <typename T>
class HttpClientConnection
{
private:
    int32_t m_socket;
    std::shared_ptr<T> m_router;

public:
    HttpClientConnection(int32_t socket, const std::shared_ptr<T> &router) : m_socket(socket),
                                                                             m_router(std::move(router)){};
    ~HttpClientConnection() {}

    void start();
};

template <typename T>
void HttpClientConnection<T>::start()
{
    fd_set readfds;
    int max_sd, activity;

    while (true)
    {
        // Clear the file descriptor set and add the client socket
        FD_ZERO(&readfds);
        FD_SET(m_socket, &readfds);
        max_sd = m_socket;

        // Wait for activity on any of the file descriptors
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }

        // If the client socket has data to read
        if (FD_ISSET(m_socket, &readfds))
        {
            std::string request;
            std::vector<char> buffer(MAX_BUFFER_SIZE);

            while (true)
            {
                int bytes_read = read(m_socket, buffer.data(), buffer.size());
                if (bytes_read < 0)
                {
                    perror("read");
                    break; // Exit the thread if read fails
                }
                else if (bytes_read == 0)
                {
                    std::cout << "Client disconnected I." << std::endl;
                    break; // Exit the thread if the client disconnected
                }

                request.append(buffer.data(), bytes_read);

                if (bytes_read < buffer.size())
                {
                    // We've read all the available data, so exit the loop
                    break;
                }

                // Double the buffer size for the next read
                buffer.resize(buffer.size() * 2);
            }

            std::string response = m_router->handle_request(request);

            int bytes_sent = send(m_socket, response.c_str(), response.size(), 0);
            if (bytes_sent < 0)
            {
                perror("send");
                break; // Exit the thread if send fails
            }
            else if (bytes_sent == 0)
            {
                std::cout << "Client disconnected II." << std::endl;
                break; // Exit the thread if the client disconnected
            }
        }
        else
        {
            // If the socket was closed, exit the thread
            break;
        }
    }

    close(m_socket);
}

#endif