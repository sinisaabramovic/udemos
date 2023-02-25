#ifndef HTTP_LOGGER_HPP
#define HTTP_LOGGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include "ud_logger_type.hpp"

class ud_logger_mediator;

class ud_logger
{
public:
    ud_logger(std::shared_ptr<ud_logger_mediator> mediator) : m_mediator(mediator) {}

    void log(ud_log_level level, const std::string &message);

private:
    std::shared_ptr<ud_logger_mediator> m_mediator;
};

#endif