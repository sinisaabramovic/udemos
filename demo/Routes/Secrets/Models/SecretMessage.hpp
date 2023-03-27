//
//  SecretMessage.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 25.03.2023..
//

#ifndef SecretMessage_hpp
#define SecretMessage_hpp

#include <string>
#include <chrono>
#include <uuid/uuid.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SecretMessage {
    std::string message;
    std::string createdAt;
    uuid_t id;

    SecretMessage(const std::string& message) {
        this->message = message;
        this->createdAt = getFormattedTime();
        uuid_generate(this->id);
    }

    std::string getFormattedTime() const {
        auto currentTime = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(currentTime);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // Define a to_json method to convert the struct to JSON
    void to_json(json& j) const {
        j = json{
            {"message", message},
            {"createdAt", createdAt},
            {"id", uuidToString(id)}
        };
    }

    // Define a from_json method to convert the JSON to the struct
    void from_json(const json& j) {
        message = j.at("message").get<std::string>();
        createdAt = j.at("createdAt").get<std::string>();
        std::string idStr = j.at("id").get<std::string>();
        uuid_parse(idStr.c_str(), id);
    }

    // Helper function to convert a uuid_t to a string
    std::string uuidToString(const uuid_t& id) const {
        char idStr[37];
        uuid_unparse(id, idStr);
        return std::string(idStr);
    }
};

// Define a to_json method for the SecretMessage struct to be used by the nlohmann::json library
void to_json(json& j, const SecretMessage& secretMessage) {
    secretMessage.to_json(j);
}

// Define a from_json method for the SecretMessage struct to be used by the nlohmann::json library
void from_json(const json& j, SecretMessage& secretMessage) {
    secretMessage.from_json(j);
}

std::string getFormattedTime() {
    auto currentTime = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(currentTime);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

#endif /* SecretMessage_hpp */
