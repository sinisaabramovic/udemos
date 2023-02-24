#ifndef HTTPS_CLIENT_CONNECTION_HPP
#define HTTPS_CLIENT_CONNECTION_HPP

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../Defines/HttpDefines.h"
#include <memory>
#include <string>
#include <sys/socket.h>
#include "../Router/HttpRouter.hpp"

template <typename T>
class HttpsClientConnection
{
private:
    SSL *m_ssl;
    std::shared_ptr<T> m_router;
    int32_t m_socket;

public:
    HttpsClientConnection(int32_t socket, const std::shared_ptr<T> &router, SSL_CTX *ssl_ctx) : m_router(std::move(router)), m_socket(socket)
    {
        m_ssl = SSL_new(ssl_ctx);
        SSL_set_fd(m_ssl, socket);
        SSL_accept(m_ssl);
    }
    ~HttpsClientConnection()
    {
        SSL_shutdown(m_ssl);
        SSL_free(m_ssl);
    };

    void start();
};

template <typename T>
void HttpsClientConnection<T>::start()
{
    while (true)
    {
        std::string request;
        std::vector<char> buffer(MAX_BUFFER_SIZE);

        int bytes_read = SSL_read(m_ssl, buffer.data(), buffer.size());
        if (bytes_read < 0)
        {
            int err = SSL_get_error(m_ssl, bytes_read);
            if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE)
            {
                continue;
            }
            else
            {
                perror("SSL_read");
                break; // Exit the thread if SSL_read fails
            }
        }
        else if (bytes_read == 0)
        {
            std::cout << "HTTPS Client disconnected" << std::endl;
            break; // Exit the thread if the client disconnected
        }

        request.append(buffer.data(), bytes_read);

        std::string response = m_router->handle_request(request);

        int bytes_sent = SSL_write(m_ssl, response.c_str(), response.size());
        if (bytes_sent < 0)
        {
            int err = SSL_get_error(m_ssl, bytes_sent);
            if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE)
            {
                continue;
            }
            else
            {
                perror("SSL_write");
                break; // Exit the thread if SSL_write fails
            }
        }
        else if (bytes_sent == 0)
        {
            std::cout << "HTTPS Client disconnected" << std::endl;
            break; // Exit the thread if the client disconnected
        }
    }

    SSL_shutdown(m_ssl);
    SSL_free(m_ssl);
    close(m_socket);
}

#endif