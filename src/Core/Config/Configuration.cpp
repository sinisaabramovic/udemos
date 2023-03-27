//
//  Configuration.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Configuration.hpp"

Configuration& Configuration::getInstance() {
    static Configuration instance;
    return instance;
}

void Configuration::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open configuration file: " + filePath);
    }
    file >> config_;
}

void Configuration::loadFromEnvironment() {
    for (const auto& [key, value] : config_.items()) {
        const char* envValue = std::getenv(key.c_str());
        if (envValue) {
            config_[key] = envValue;
        }
    }
}

void Configuration::loadFromCommandLine(int argc, char* argv[]) {
    std::unordered_map<std::string, std::string> args;
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        size_t pos = arg.find('=');
        if (pos != std::string::npos) {
            std::string key = arg.substr(0, pos);
            std::string value = arg.substr(pos + 1);
            args[key] = value;
        }
    }

    for (const auto& [key, value] : args) {
        if (config_.find(key) != config_.end()) {
            config_[key] = value;
        }
    }
}

template<typename T>
T Configuration::get(const std::string& key) const {
    if (config_.find(key) == config_.end()) {
        throw std::runtime_error("Invalid configuration key: " + key);
    }
    return config_[key].get<T>();
}

template<typename T>
void Configuration::set(const std::string& key, const T& value) {
    config_[key] = value;
}

void Configuration::set(const std::string& key, const char* value) {
    config_[key] = std::string(value);
}

template int Configuration::get<int>(const std::string&) const;
template std::string Configuration::get<std::string>(const std::string&) const;
template bool Configuration::get<bool>(const std::string&) const;

template void Configuration::set<int>(const std::string&, const int&);
template void Configuration::set<std::string>(const std::string&, const std::string&);
template void Configuration::set<bool>(const std::string&, const bool&);

template uint16_t Configuration::get<uint16_t>(const std::string&) const;
template void Configuration::set<uint16_t>(const std::string&, const uint16_t&);


