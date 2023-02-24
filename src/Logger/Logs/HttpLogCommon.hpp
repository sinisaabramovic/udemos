#ifndef HTTP_LOG_COMMON_HPP
#define HTTP_LOG_COMMON_HPP

#include "../HttpLogType.hpp"

class HttpLogCommon : public HttpLog
{
public:
    HttpLogCommon(HttpLogLevel level, const std::string &message) : HttpLog(level, message) {}
    ~HttpLogCommon() = default;

    std::string description() const
    {
        return "COMMON " + this->description();
    }
};

#endif