#ifndef HTTP_LOGGER_HPP
#define HTTP_LOGGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include "HttpLogType.hpp"

class HttpLoggerMediator;

class HttpLogger
{
public:
    HttpLogger(std::shared_ptr<HttpLoggerMediator> mediator) : m_mediator(mediator) {}

    void log(HttpLogLevel level, const std::string &message);

private:
    std::shared_ptr<HttpLoggerMediator> m_mediator;
};

#endif