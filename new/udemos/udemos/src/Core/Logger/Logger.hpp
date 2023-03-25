//
//  Logger.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef Logger_hpp
#define Logger_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <mutex>

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static Logger& getInstance();
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void setLogLevel(LogLevel level);
    void log(LogLevel level, const std::string& message);
    void setLogFile(const std::string& filePath);
    
private:
    Logger();
    
    LogLevel logLevel_;
    std::unique_ptr<std::ofstream> logFile_;
    std::mutex logMutex_;
};


#endif /* Logger_hpp */
