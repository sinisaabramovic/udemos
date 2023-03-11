#ifndef ud_http_connection_data_hpp
#define ud_http_connection_data_hpp

#include <memory>
#include <string>
#include <sys/socket.h>

#include "../common/ud_http_defines.h"
#include "../router/ud_http_router.hpp"

#include "../socket/ud_http_socket.hpp"
#include "../common/result/ud_common_result.hpp"

template <typename T>
class ud_http_connection_data
{
public:
    ud_http_connection_data(int32_t socket, const std::shared_ptr<T> &router) : m_socket(std::make_shared<ud_http_socket>(socket)),
                                                                                m_router(std::move(router)){};

    std::shared_ptr<ud_http_socket> m_socket;
    std::shared_ptr<T> m_router;
};

#endif