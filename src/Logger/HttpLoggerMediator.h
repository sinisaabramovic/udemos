#ifndef HTTP_LOGGER_MEDIATOR_H
#define HTTP_LOGGER_MEDIATOR_H

#include "HttpLogType.hpp"

class HttpLogger;

class HttpLoggerMediator : public std::enable_shared_from_this<HttpLoggerMediator>
{
public:
    void add_logger(std::shared_ptr<HttpLogger> logger);
    void notify(std::shared_ptr<HttpLog> logType);

private:
    std::vector<std::shared_ptr<HttpLogger>> m_loggers;
    std::vector<std::shared_ptr<HttpLog>> m_logTypes;
};

#endif