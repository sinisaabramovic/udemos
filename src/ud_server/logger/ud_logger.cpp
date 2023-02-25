#include "ud_logger.h"
#include "ud_logger_mediator.h"

void ud_logger::log(ud_logger level, const std::string &message)
{
    auto log_type = std::make_shared<ud_log>(level, message);
    m_mediator->notify(log_type);
}