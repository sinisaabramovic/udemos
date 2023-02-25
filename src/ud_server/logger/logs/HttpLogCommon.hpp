#ifndef HTTP_LOG_COMMON_HPP
#define HTTP_LOG_COMMON_HPP

#include "../ud_logger_type.hpp"

class ud_common_log : public ud_log
{
public:
    ud_common_log(ud_log_level level, const std::string &message) : ud_log(level, message) {}
    ~ud_common_log() = default;

    std::string description() const
    {
        return "COMMON " + this->description();
    }
};

#endif