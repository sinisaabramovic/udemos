//
//  SimpleSecretMessage.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 28.03.2023..
//

#ifndef SimpleSecretMessage_hpp
#define SimpleSecretMessage_hpp

#include <string>
#include <chrono>
#include <uuid/uuid.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SimpleSecretMessage {
    std::string message;

    SimpleSecretMessage(const std::string& message) {
        this->message = message;
    }

    // Define a to_json method to convert the struct to JSON
    void to_json(json& j) const {
        j = json{
            {"message", message}
        };
    }

    // Define a from_json method to convert the JSON to the struct
    void from_json(const json& j) {
        message = j.at("message").get<std::string>();
    }

};

// Define a to_json method for the SecretMessage struct to be used by the nlohmann::json library
void to_json(json& j, const SimpleSecretMessage& secretMessage) {
    secretMessage.to_json(j);
}

// Define a from_json method for the SecretMessage struct to be used by the nlohmann::json library
void from_json(const json& j, SimpleSecretMessage& secretMessage) {
    secretMessage.from_json(j);
}

#endif /* SimpleSecretMessage_hpp */
