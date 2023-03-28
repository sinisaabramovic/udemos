//
//  GetSimpleSecretRouteHandler.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 28.03.2023..
//

#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>


#include "../../../src/Protocol/HttpModels/HttpResponse.hpp"
#include "../../../src/Protocol/HttpModels/HttpRequest.hpp"
#include "GetSimpleSecretRouteHandler.hpp"
#include "../../../src/Crypto/AESCipher.hpp"
#include "../../../src/Core/Logger/Logger.hpp"

#include <iostream>
#include <nlohmann/json.hpp>
#include "Models/SimpleSecretMessage.hpp"

GetSimpleSecretRouteHandler::GetSimpleSecretRouteHandler(const std::string& method, const std::string& path) : RouteHandler(method, path) {}

std::shared_ptr<HttpResponse> GetSimpleSecretRouteHandler::handleRequest(const HttpRequest& request) {
    
    auto response = std::make_shared<HttpResponse>();
    response->setStatusCode(HttpResponseStatus::OK);
    response->addHeader("Content-Type", "application/json");
    
    OpenSSL_add_all_algorithms();
    
    SimpleSecretMessage secretMessage("Hello. Some simple message");
    json secretMessageJson = secretMessage;
    
    const std::string plaintext = secretMessageJson.dump();
    try {
        AESCipher cipher("usr/share/keys/private_key.pem");
        auto responsePayload = cipher.encrypt(reinterpret_cast<const byte*>(plaintext.data()), plaintext.size());
        std::stringstream chiperHexString;
        for (const auto& byte : responsePayload) {
            chiperHexString << std::hex << static_cast<int>(byte);
        }
        
        auto decrypted64RequestBody = cipher.base64Decode(request.getBody());
        auto decryptedData = cipher.decrypt(decrypted64RequestBody.data(), decrypted64RequestBody.size());
        std::string decryptedText(reinterpret_cast<char*>(decryptedData.data()), decryptedData.size());
        Logger::getInstance().log(LogLevel::Info, "Decrypted request data: " + decryptedText);
        
        auto encodedPayload = cipher.base64Encode(responsePayload);
        response->setStatusCode(HttpResponseStatus::OK);
        response->setBody(encodedPayload);
        
    } catch (const std::exception& ex) {
        Logger::getInstance().log(LogLevel::Error, "Failed in AESCipher: " + std::string(ex.what()));
        response->setStatusCode(HttpResponseStatus::BAD_REQUEST);
        response->setBody("Failed to create payload");
    }
    
    EVP_cleanup();
    
    return response;
}
