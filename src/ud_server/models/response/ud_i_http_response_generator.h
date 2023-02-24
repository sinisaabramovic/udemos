#ifndef ud_i_http_response_generator_hpp
#define ud_i_http_response_generator_hpp

#include <iostream>
#include <string>

enum class ud_http_status_codes : uint16_t 
{
    OK = 200,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    INTERVALE_SERVER_ERROR = 500,
};

class ud_i_http_response_generator {
public:
    virtual std::string create_generator(ud_http_status_codes status_code, const std::string& content_type, const std::string& payload) = 0;
};

#endif