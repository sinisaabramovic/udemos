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

    
    bool send_response_to_client(const std::string& response);
    std::string read_request_from_client();
    bool handle_client_request(fd_set* readfds);
    int wait_for_activity(int max_sd, fd_set* readfds);

public:
    ud_http_connection(int32_t socket, const std::shared_ptr<T> &router) : 
        m_socket(std::make_shared<ud_http_socket>(socket)),
        m_router(std::move(router)){};
    ~ud_http_connection() {}

    void start();
};

template <typename T>
void ud_http_connection<T>::start() {
    while (true) {
        if (this->m_socket->get_socket() <= 0) {
            break;
        }

        fd_set readfds;
        int max_sd = this->m_socket->get_socket();
        FD_ZERO(&readfds);
        FD_SET(max_sd, &readfds);

        int activity = wait_for_activity(max_sd, &readfds);

        if (activity < 0) {
            perror("select error");
            break;
        }

        if (activity == 0) {
            // The timeout has expired, do nothing
            continue;
        }

        if (!handle_client_request(&readfds)) {
            // An error occurred while handling the client request
            break;
        }
    }

    this->m_socket->close_socket();
}

template <typename T>
int ud_http_connection<T>::wait_for_activity(int max_sd, fd_set* readfds) {
    // Wait for activity on any of the file descriptors
    return select(max_sd + 1, readfds, nullptr, nullptr, nullptr);
}

template <typename T>
bool ud_http_connection<T>::handle_client_request(fd_set* readfds) {
    // If the client socket has data to read
    if (!FD_ISSET(this->m_socket->get_socket(), readfds)) {
        // The socket was closed, exit the thread
        return false;
    }

    std::string request = read_request_from_client();
    if (request.empty()) {
        // An error occurred while reading the request
        return false;
    }

    std::string response = m_router->handle_request(request);
    if (response.empty()) {
        // An error occurred while handling the request
        return false;
    }

    if (!send_response_to_client(response)) {
        // An error occurred while sending the response
        return false;
    }

    return true;
}

template <typename T>
std::string ud_http_connection<T>::read_request_from_client() {
    std::string request;
    std::vector<char> buffer(MAX_BUFFER_SIZE);

    while (true) {
        int bytes_read = read(this->m_socket->get_socket(), buffer.data(), buffer.size());
        if (bytes_read < 0) {
            perror("read");
            this->m_socket->close_socket();
            return "";
        } else if (bytes_read == 0) {
            this->m_socket->close_socket();
            std::cout << "Client disconnected I." << std::endl;
            return "";
        }

        request.append(buffer.data(), bytes_read);

        if (bytes_read < buffer.size()) {
            // We've read all the available data, so exit the loop
            break;
        }

        // Double the buffer size for the next read
        buffer.resize(buffer.size() * 2);
    }

    return request;
}

template <typename T>
bool ud_http_connection<T>::send_response_to_client(const std::string& response) {
    int bytes_sent = send(this->m_socket->get_socket(), response.c_str(), response.size(), 0);
    if (bytes_sent < 0) {
        perror("send");
        this->m_socket->close_socket();
        return false;
    } else if (bytes_sent == 0) {
        std::cout << "Client disconnected II." << std::endl;
        this->m_socket->close_socket();
        return false;
    }

    return true;
}


#endif