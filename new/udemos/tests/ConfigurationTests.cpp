//
//  ConfigurationTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "Configuration.hpp"

TEST(ConfigurationTests, LoadFromFile) {
    Configuration& config = Configuration::getInstance();

    // Create a temporary configuration file
    std::string filePath = "config_test_load_file.json";
    std::ofstream file(filePath);
    file << R"({
        "port": 8080,
        "host": "127.0.0.1",
        "enable_https": false,
        "protocol": "http"
    })";
    file.close();

    // Load the configuration from the file
    config.loadFromFile(filePath);

    // Check if the settings are correct
    EXPECT_EQ(config.get<int>("port"), 8080);
    EXPECT_EQ(config.get<std::string>("host"), "127.0.0.1");
    EXPECT_EQ(config.get<bool>("enable_https"), false);

    // Remove the temporary configuration file
    std::remove(filePath.c_str());
}

TEST(ConfigurationTests, LoadFromEnvironment) {
    Configuration& config = Configuration::getInstance();

    // Set environment variables
    setenv("port", "8081", 1);
    setenv("host", "localhost", 1);
    setenv("protocol", "http", 1);

    // Load the configuration from the environment variables
    config.loadFromEnvironment();

    // Check if the settings are correct
    EXPECT_EQ(config.get<std::string>("port"), "8081");
    EXPECT_EQ(config.get<std::string>("host"), "localhost");
    EXPECT_EQ(config.get<std::string>("protocol"), "http");

    // Clean up environment variables
    unsetenv("port");
    unsetenv("host");
    unsetenv("protocol");
}

TEST(ConfigurationTests, LoadFromCommandLine) {
    Configuration& config = Configuration::getInstance();

    // Prepare command-line arguments
    int argc = 4;
    char* argv[] = {
        (char*)"program_name",
        (char*)"port=8082",
        (char*)"host=192.168.1.1",
        (char*)"enable_https=true"
    };

    // Load the configuration from the command-line arguments
    config.loadFromCommandLine(argc, argv);

    // Check if the settings are correct
    EXPECT_EQ(config.get<std::string>("port"), "8082");
    EXPECT_EQ(config.get<std::string>("host"), "192.168.1.1");

    // Convert the "enable_https" setting to boolean
    std::string enable_https_str = config.get<std::string>("enable_https");
    bool enable_https = (enable_https_str == "true" || enable_https_str == "1");
    EXPECT_EQ(enable_https, true);
}

TEST(ConfigurationTests, SetAndGet) {
    Configuration& config = Configuration::getInstance();

    // Set and get settings
    config.set<int>("port", 9090);
    config.set<std::string>("host", "10.0.0.1");
    config.set<bool>("enable_https", true);

    EXPECT_EQ(config.get<int>("port"), 9090);
    EXPECT_EQ(config.get<std::string>("host"), "10.0.0.1");
    EXPECT_EQ(config.get<bool>("enable_https"), true);
}
