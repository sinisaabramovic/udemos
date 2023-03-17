//
//  Logger.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "Logger.hpp"

Logger::Logger() : logLevel_(LogLevel::Info) {}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level) {
    logLevel_ = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < logLevel_) {
        return;
    }
    
    std::unique_lock<std::mutex> lock(logMutex_);
    
    if (logFile_) {
        (*logFile_) << message << std::endl;
    } else {
        std::cout << message << std::endl;
    }
}

void Logger::setLogFile(const std::string& filePath) {
    logFile_ = std::make_unique<std::ofstream>(filePath);
    if (!logFile_->is_open()) {
        throw std::runtime_error("Failed to open log file: " + filePath);
    }
}
