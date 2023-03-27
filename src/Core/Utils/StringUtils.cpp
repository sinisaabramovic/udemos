//
//  StringUtils.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "StringUtils.hpp"

namespace StringUtils {

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toUpper(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string trim(const std::string& input) {
    size_t first = input.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = input.find_last_not_of(" \t\n\r");
    return input.substr(first, (last - first + 1));
}

}
