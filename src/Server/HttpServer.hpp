// #ifndef HTTP_SERVER_HPP
// #define HTTP_SERVER_HPP

// #include <iostream>
// #include <string>
// #include <vector>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <thread>
// #include <chrono>

// #include <openssl/ssl.h>
// #include <openssl/err.h>

// #include "../Router/HttpRouter.hpp"
// #include "../Common/HttpThreadPool.hpp"
// #include "../Connection/HttpClientConnection.hpp"
// #include "../Connection/HttpsClientConnection.hpp"
// #include "../Acceptor/HttpAcceptor.hpp"

// template <typename T>
// class HttpServer
// {
// private:
//     uint32_t m_port;
//     std::shared_ptr<T> m_router;
//     std::unique_ptr<HttpThreadPool> m_thread_pool;
//     std::unique_ptr<HttpAcceptor> m_acceptor;
//     std::string m_cert_path;
//     std::string m_key_path;

//     SSL_CTX *m_ssl_ctx = nullptr;

//     void setup_ssl();

// public:
//     HttpServer(uint32_t port, std::shared_ptr<T> router, const std::string& cert_path = "", const std::string& key_path = "", bool use_ssl = false, const std::vector<std::string> &ip_address_to_ignore = {})
//         : m_port(port),
//           m_router(std::move(router)),
//           m_thread_pool(std::make_unique<HttpThreadPool>(4)),
//           m_acceptor(std::make_unique<HttpAcceptor>(port, ip_address_to_ignore))
//     {
//         if (use_ssl)
//         {
//             this->m_cert_path = cert_path;
//             this->m_key_path = key_path;
//             setup_ssl();
//         }
//     }

//     ~HttpServer()
//     {
//         if (m_ssl_ctx)
//         {
//             SSL_CTX_free(m_ssl_ctx);            
//             EVP_cleanup();
//         }
//     };

//     void start();
// };

// template <typename T>
// void HttpServer<T>::start()
// {
//     int32_t new_socket;
//     std::cout << "udemos server started on port " << m_port << std::endl;
//     while (true)
//     {
//         if ((new_socket = m_acceptor->accept_connection()) < 0)
//         {
//             perror("accept");
//             std::cout << "KILLED \n"; 
//             exit(EXIT_FAILURE);
//         }

//         if (m_ssl_ctx)
//         {
//             SSL *ssl = SSL_new(m_ssl_ctx);
//             SSL_set_fd(ssl, new_socket);
//             if (SSL_accept(ssl) <= 0)
//             {
//                 ERR_print_errors_fp(stderr);
//                 SSL_shutdown(ssl);
//                 SSL_free(ssl);
//                 close(new_socket);
//                 continue;
//             }
//             std::shared_ptr<HttpsClientConnection<T>> client_connection = std::make_shared<HttpsClientConnection<T>>(new_socket, m_router, m_ssl_ctx);
//             m_thread_pool->enqueue(&HttpsClientConnection<T>::start, client_connection);
//         }
//         else
//         {
//             std::shared_ptr<HttpClientConnection<T>> client_connection = std::make_shared<HttpClientConnection<T>>(new_socket, m_router);
//             m_thread_pool->enqueue(&HttpClientConnection<T>::start, client_connection);
//         }
//     }
// }

// template <typename T>
// void HttpServer<T>::setup_ssl()
// {
//     SSL_library_init();
//     OpenSSL_add_all_algorithms();
//     SSL_load_error_strings();

//     m_ssl_ctx = SSL_CTX_new(TLS_server_method());
//     if (!m_ssl_ctx)
//     {
//         throw std::runtime_error("SSL_CTX_new() failed");
//     }

//     SSL_CTX_set_verify(m_ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);

//     std::cout << "CERT FILE: " << m_cert_path.c_str() << "\n";
//     std::cout << "KEY FILE :" << m_key_path.c_str() << "\n";

//     if (SSL_CTX_use_certificate_file(m_ssl_ctx, m_cert_path.c_str(), SSL_FILETYPE_PEM) <= 0)
//     {
//         throw std::runtime_error("SSL_CTX_use_certificate_file() failed");
//     }

//     if (SSL_CTX_use_PrivateKey_file(m_ssl_ctx, m_key_path.c_str(), SSL_FILETYPE_PEM) <= 0)
//     {
//         throw std::runtime_error("SSL_CTX_use_PrivateKey_file() failed");
//     }
// }

// #endif