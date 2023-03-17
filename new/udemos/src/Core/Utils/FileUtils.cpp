//
//  FileUtils.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <fstream>
#include "FileUtils.hpp"

namespace FileUtils {

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    file << content;
}

}  // namespace FileUtils
