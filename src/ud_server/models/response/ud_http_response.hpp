#ifndef ud_http_response_hpp
#define ud_http_response_hpp

#include <string>
#include <utility>
#include <iostream>
#include <map>
#include <unordered_map>
#include "util/ud_http_response_utils.hpp"

enum class ud_response_status_code
{
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

class ud_http_response
{
public:
    ud_http_response(ud_response_status_code code, const std::string &content_type, const std::string &body) 
    : m_status_code(code), m_content_type(content_type), m_body(body) {}
    ud_http_response() : m_status_code(ud_response_status_code::OK), m_content_type("text/plain"), m_body("") {}
    ~ud_http_response() {}

    std::string to_string();

    ud_response_status_code get_status_code() const;
    void set_status_code(ud_response_status_code status_code);

    std::string get_content_type() const;
    void set_content_type(const std::string& content_type);

    std::string get_body() const;
    void set_body(const std::string& body);

    std::unordered_map<std::string, std::string> get_headers() const;
    void set_headers(std::unordered_map<std::string, std::string> headers);
    void add_header(const std::string& name, const std::string& value);

private:
    ud_response_status_code m_status_code;
    std::string m_content_type;
    std::string m_body;
    std::unordered_map<std::string, std::string> m_headers;

    std::string get_status_description(ud_response_status_code code) const
    {
        switch (code)
        {
        case ud_response_status_code::OK:
            return "OK";
        case ud_response_status_code::CREATED:
            return "Created";
        case ud_response_status_code::ACCEPTED:
            return "Accepted";
        case ud_response_status_code::NO_CONTENT:
            return "No Content";
        case ud_response_status_code::MOVED_PERMANENTLY:
            return "Moved Permanently";
        case ud_response_status_code::FOUND:
            return "Found";
        case ud_response_status_code::SEE_OTHER:
            return "See Other";
        case ud_response_status_code::NOT_MODIFIED:
            return "Not Modified";
        case ud_response_status_code::TEMPORARY_REDIRECT:
            return "Temporary Redirect";
        case ud_response_status_code::PERMANENT_REDIRECT:
            return "Permanent Redirect";
        case ud_response_status_code::BAD_REQUEST:
            return "Bad Request";
        case ud_response_status_code::UNAUTHORIZED:
            return "Unauthorized";
        case ud_response_status_code::FORBIDDEN:
            return "Forbidden";
        case ud_response_status_code::NOT_FOUND:
            return "Not Found";
        case ud_response_status_code::METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case ud_response_status_code::INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        case ud_response_status_code::NOT_IMPLEMENTED:
            return "Not Implemented";
        case ud_response_status_code::BAD_GATEWAY:
            return "Bad Gateway";
        case ud_response_status_code::SERVICE_UNAVAILABLE:
            return "Service Unavailable";
        case ud_response_status_code::GATEWAY_TIMEOUT:
            return "Gateway Timeout";
        case ud_response_status_code::HTTP_VERSION_NOT_SUPPORTED:
            return "HTTP Version Not Supported";
        default:
            return "";
        }
    }
};

ud_response_status_code ud_http_response::get_status_code() const
{
    return this->m_status_code;
}

void ud_http_response::set_status_code(ud_response_status_code status_code)
{
    this->m_status_code = status_code;
}

std::string ud_http_response::get_content_type() const
{
    return this->m_content_type;
}

void ud_http_response::set_content_type(const std::string& content_type)
{
    this->m_content_type = content_type;
}

std::string ud_http_response::get_body() const
{
    return this->m_body;
}

void ud_http_response::set_body(const std::string& body)
{
    this->m_body = body;
}

std::unordered_map<std::string, std::string> ud_http_response::get_headers() const
{
    return m_headers;
}

void ud_http_response::set_headers(std::unordered_map<std::string, std::string> headers)
{
    for (const auto& [header_name, header_value] : headers) {
      this->add_header(header_name, header_value);
    }
}

void ud_http_response::add_header(const std::string& name, const std::string& value)
{
    this->m_headers[name] = value;
}

std::string ud_http_response::to_string()
{
    // TODO: refactor this - currently we set default headers
    for (const auto& [header_name, header_value] : ud_http_response_utils::cors_headers_default()) {
      this->add_header(header_name, header_value);
    }
    //

    std::string status_description = get_status_description(m_status_code);
    std::string response = "HTTP/1.1 " + std::to_string(static_cast<int>(m_status_code)) + " " + status_description + "\r\n";
    response += "Content-Length: " + std::to_string(m_body.length()) + "\r\n";
    for (auto &[name, value] : m_headers)
    {
        response += name + ": " + value + "\r\n";
    }
    response += "\r\n" + m_body;
    return response;
}

#endif // ud_http_response_hpp
