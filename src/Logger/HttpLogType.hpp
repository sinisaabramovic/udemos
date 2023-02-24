#ifndef HTTP_LOGTYPE_HPP
#define HTTP_LOGTYPE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

enum class HttpLogLevel
{
    INFO,
    WARNING,
    ERROR,
    FATAL_ERROR
};

class HttpLog
{
public:
    HttpLog(HttpLogLevel level, const std::string &message) : m_level(level), m_message(message), m_createdAt(std::chrono::system_clock::now()) {}

    HttpLogLevel get_level() const { return m_level; }
    std::string get_message() const { return m_message; }
    std::chrono::system_clock::time_point get_createdAt() const { return m_createdAt; }

    std::string description() const
    {
        std::string levelString;
        switch (m_level)
        {
        case HttpLogLevel::INFO:
            levelString = "INFO";
            break;
        case HttpLogLevel::WARNING:
            levelString = "WARNING";
            break;
        case HttpLogLevel::ERROR:
            levelString = "ERROR";
            break;
        case HttpLogLevel::FATAL_ERROR:
            levelString = "FATAL_ERROR";
            break;
        }

        std::chrono::system_clock::duration elapsedTime = std::chrono::system_clock::now() - m_createdAt;
        std::string elapsedString = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count()) + "ms";

        return levelString + ": " + "created: " + elapsedString + " " + m_message;
    }

protected:
    HttpLogLevel m_level;
    std::string m_message;
    std::chrono::system_clock::time_point m_createdAt;
};

#endif