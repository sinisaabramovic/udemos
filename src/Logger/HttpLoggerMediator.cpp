#include "HttpLoggerMediator.h"
#include "HttpLogger.h"

void HttpLoggerMediator::add_logger(std::shared_ptr<HttpLogger> logger)
{
    m_loggers.emplace_back(logger);
}

void HttpLoggerMediator::notify(std::shared_ptr<HttpLog> logType)
{
    m_logTypes.emplace_back(logType);
    std::cout << logType->description() << std::endl;
}