#ifndef ud_http_connection_hpp
#define ud_http_connection_hpp

#include <memory>
#include <string>
#include <sys/socket.h>

#include "../common/udp_http_defines.h"
#include "../router/ud_http_router.hpp"

#include "../socket/ud_http_socket.hpp"

template <typename T>
class ud_http_connection
{
private:
    std::shared_ptr<ud_http_socket> m_socket;
    std::shared_ptr<T> m_router;

public:
    ud_http_connection(int32_t socket, const std::shared_ptr<T> &router) : 
        m_socket(std::make_shared<ud_http_socket>(socket)),
        m_router(std::move(router)){};
    ~ud_http_connection() {}

    void start();
};

template <typename T>
void ud_http_connection<T>::start()
{
    fd_set readfds;
    int max_sd, activity;

    while (true)
    {
        if (this->m_socket->get_socket() <= 0) 
        {
            break;
        }
        // Clear the file descriptor set and add the client socket
        FD_ZERO(&readfds);
        FD_SET(this->m_socket->get_socket(), &readfds);
        max_sd = this->m_socket->get_socket();

        // Wait for activity on any of the file descriptors
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }

        // If the client socket has data to read
        if (FD_ISSET(this->m_socket->get_socket(), &readfds))
        {
            std::string request;
            std::vector<char> buffer(MAX_BUFFER_SIZE);

            while (true)
            {
                int bytes_read = read(this->m_socket->get_socket(), buffer.data(), buffer.size());
                if (bytes_read < 0)
                {
                    perror("read");
                    this->m_socket->close_socket();
                    break; // Exit the thread if read fails
                }
                else if (bytes_read == 0)
                {
                    this->m_socket->close_socket();
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

            int bytes_sent = send(this->m_socket->get_socket(), response.c_str(), response.size(), 0);
            if (bytes_sent < 0)
            {
                perror("send");
                this->m_socket->close_socket();
                break; // Exit the thread if send fails
            }
            else if (bytes_sent == 0)
            {
                std::cout << "Client disconnected II." << std::endl;
                this->m_socket->close_socket();
                break; // Exit the thread if the client disconnected
            }
        }
        else
        {
            // If the socket was closed, exit the thread
            break;
        }
    }

    this->m_socket->close_socket();
}

#endif