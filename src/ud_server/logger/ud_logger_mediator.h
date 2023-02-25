#ifndef ud_logger_mediator_h
#define ud_logger_mediator_h

#include "ud_logger_type.hpp"

class ud_logger;

class ud_logger_mediator : public std::enable_shared_from_this<ud_logger_mediator>
{
public:
    void add_logger(std::shared_ptr<ud_logger> logger);
    void notify(std::shared_ptr<ud_log> logType);

private:
    std::vector<std::shared_ptr<ud_logger>> m_loggers;
    std::vector<std::shared_ptr<ud_log>> m_log_types;
};

#endif