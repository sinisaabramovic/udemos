//
//  LoggerTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <gtest/gtest.h>
#include <cstdio>
#include "Logger.hpp"
#include "TestUtils.h"

TEST(LoggerTests, LogLevelFiltering) {
    Logger& logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::Warning);
    
    // Set log output to file
    std::string logFilePath = "log_test_filtering.txt";
    logger.setLogFile(logFilePath);
    
    // Log messages
    logger.log(LogLevel::Debug, "This debug message should not appear");
    logger.log(LogLevel::Info, "This info message should not appear");
    logger.log(LogLevel::Warning, "This warning message should appear");
    logger.log(LogLevel::Error, "This error message should appear");
    
    // Read content from log file
    std::string logContent = TestUtils::readLogFile(logFilePath);
    
    // Check if the log content contains the expected messages
    EXPECT_EQ(logContent.find("This debug message should not appear"), std::string::npos);
    EXPECT_EQ(logContent.find("This info message should not appear"), std::string::npos);
    EXPECT_NE(logContent.find("This warning message should appear"), std::string::npos);
    EXPECT_NE(logContent.find("This error message should appear"), std::string::npos);
    
    // Remove the test log file
    std::remove(logFilePath.c_str());
}

TEST(LoggerTests, LogFileOutput) {
    Logger& logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::Debug);
    
    // Set log output to file
    std::string logFilePath = "log_test_output.txt";
    logger.setLogFile(logFilePath);
    
    // Log messages
    logger.log(LogLevel::Debug, "This is a debug message");
    logger.log(LogLevel::Info, "This is an info message");
    logger.log(LogLevel::Warning, "This is a warning message");
    logger.log(LogLevel::Error, "This is an error message");
    
    // Read content from log file
    std::string logContent = TestUtils::readLogFile(logFilePath);
    
    // Check if the log content contains the expected messages
    EXPECT_NE(logContent.find("This is a debug message"), std::string::npos);
    EXPECT_NE(logContent.find("This is an info message"), std::string::npos);
    EXPECT_NE(logContent.find("This is a warning message"), std::string::npos);
    EXPECT_NE(logContent.find("This is an error message"), std::string::npos);
    
    // Remove the test log file
    std::remove(logFilePath.c_str());
}
