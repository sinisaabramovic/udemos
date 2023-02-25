#ifndef udp_logger_type_hpp
#define udp_logger_type_hpp

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

enum class ud_log_level
{
    INFO,
    WARNING,
    ERROR,
    FATAL_ERROR
};

class ud_log
{
public:
    ud_log(ud_log_level level, const std::string &message) :
     m_level(level), m_message(message), 
     m_createdAt(std::chrono::system_clock::now()) {}

    ud_log_level get_level() const { return m_level; }
    std::string get_message() const { return m_message; }
    std::chrono::system_clock::time_point get_createdAt() const { return m_createdAt; }

    std::string description() const
    {
        std::string levelString;
        switch (m_level)
        {
        case ud_log_level::INFO:
            levelString = "INFO";
            break;
        case ud_log_level::WARNING:
            levelString = "WARNING";
            break;
        case ud_log_level::ERROR:
            levelString = "ERROR";
            break;
        case ud_log_level::FATAL_ERROR:
            levelString = "FATAL_ERROR";
            break;
        }

        std::chrono::system_clock::duration elapsedTime = std::chrono::system_clock::now() - m_createdAt;
        std::string elapsedString = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count()) + "ms";

        return levelString + ": " + "created: " + elapsedString + " " + m_message;
    }

    ~ud_log() {}
    
protected:
    ud_log_level m_level;
    std::string m_message;
    std::chrono::system_clock::time_point m_createdAt;
};

#endif