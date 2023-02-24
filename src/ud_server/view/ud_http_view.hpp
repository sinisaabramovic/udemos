#ifndef ud_http_view_hpp
#define ud_http_view_hpp

#include <string>
#include <iostream>

class ud_http_request;

class ud_http_view
{
public:
    ud_http_view() {}
    ~ud_http_view() {}

    virtual std::string render(const ud_http_request &request) = 0;
};

#endif