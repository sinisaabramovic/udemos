#ifndef ud_http_time_hpp
#define ud_http_time_hpp

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

class ud_http_time
{
private:
    /* data */
public:
    ud_http_time(/* args */) {}
    ~ud_http_time() {}
    static std::string current_date_time()
    {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }
};

#endif