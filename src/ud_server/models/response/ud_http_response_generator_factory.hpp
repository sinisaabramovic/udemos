#ifndef ud_http_response_generator_factory_hpp
#define ud_http_response_generator_factory_hpp

#include "ud_i_http_response_generator.h"
#include "ud_map_http_response_generator.hpp"
#include <map>

class ud_http_response_generator_factory
{
public:
    static std::shared_ptr<ud_i_http_response_generator> create_generator()
    {
        std::map<ud_http_status_codes, std::string> error_map = {
            {ud_http_status_codes::OK, "OK"},
            {ud_http_status_codes::BAD_REQUEST, "Bad Request"},
            {ud_http_status_codes::UNAUTHORIZED, "Unauthorized"},
            {ud_http_status_codes::FORBIDDEN, "Forbidden"},
            {ud_http_status_codes::NOT_FOUND, "Not Found"},
            {ud_http_status_codes::INTERVALE_SERVER_ERROR, "Internal Server Error"}};
        return std::make_shared<ud_map_http_response_generator>(error_map);
    }
};

#endif