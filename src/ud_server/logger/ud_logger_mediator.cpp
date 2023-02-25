#include "ud_logger_mediator.h"
#include "ud_logger.h"

void ud_logger_mediator::add_logger(std::shared_ptr<ud_logger_mediator> logger)
{
    m_loggers.emplace_back(logger);
}

void ud_logger_mediator::notify(std::shared_ptr<HttpLog> log_type)
{
    m_log_types.emplace_back(log_type);
    std::cout << log_type->description() << std::endl;
}