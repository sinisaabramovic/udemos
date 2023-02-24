#ifndef ud_map_http_response_generator_hpp
#define ud_map_http_response_generator_hpp

#include <map>

#include "ud_i_http_response_generator.h"
#include "../../common/ud_http_time.hpp"

class ud_map_http_response_generator : public ud_i_http_response_generator
{
public:
    ud_map_http_response_generator(const std::map<ud_http_status_codes, std::string> &error_map)
        : m_error_map(error_map)
    {
    }

    virtual std::string create_generator(ud_http_status_codes status_code, const std::string& content_type, const std::string& payload) override
    {
        std::ostringstream oss;
        oss << "HTTP/1.1 " << static_cast<uint16_t>(status_code) << " " << m_error_map[status_code] << "\r\n";
        oss << "Date: " + ud_http_time::current_date_time() + "\r\n";
        oss << "Server: udemos 1.0\r\n";        
        oss << "Content-Length: " + std::to_string(payload.size()) + "\r\n";
        oss << "Content-Type: " + content_type + "\r\n";
        oss << "\n";
        oss << payload;
        oss << "\r\n";
        return oss.str();
    }

private:
    std::map<ud_http_status_codes, std::string> m_error_map;
};

#endif
