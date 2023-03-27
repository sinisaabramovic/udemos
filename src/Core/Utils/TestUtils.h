//
//  TestUtils.h
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef TestUtils_h
#define TestUtils_h

#include <string>

namespace TestUtils {

std::string readLogFile(const std::string& filePath);

}  // namespace TestUtils

// TestUtils.cpp
#include <fstream>
#include "TestUtils.h"

namespace TestUtils {

std::string readLogFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filePath);
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

}  // namespace TestUtils


#endif /* TestUtils_h */
