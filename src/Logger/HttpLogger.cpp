#include "HttpLogger.h"
#include "HttpLoggerMediator.h"

void HttpLogger::log(HttpLogLevel level, const std::string &message)
{
    auto logType = std::make_shared<HttpLog>(level, message);
    m_mediator->notify(logType);
}