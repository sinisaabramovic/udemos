//
//  GetSecretRouteHandler.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 21.03.2023..
//

#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

#include "../../src/Protocol/HttpModels/HttpResponse.hpp"
#include "../../src/Protocol/HttpModels/HttpRequest.hpp"
#include "GetSecretRouteHandler.hpp"
#include "../../src/Crypto/AESCipher.hpp"
#include "../../src/Core/Logger/Logger.hpp"

GetSecretRouteHandler::GetSecretRouteHandler(const std::string& method, const std::string& path) : RouteHandler(method, path) {}

std::shared_ptr<HttpResponse> GetSecretRouteHandler::handleRequest(const HttpRequest& request) {
    
    auto response = std::make_shared<HttpResponse>();
    response->setStatusCode(HttpResponseStatus::OK);
    response->addHeader("Content-Type", "application/json");
    
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    auto strTime = oss.str();
    
    OpenSSL_add_all_algorithms();
    
    const std::string plaintext = "{ message: World " + strTime + "}";
    try {
        AESCipher cipher("setecastronomy");
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
        Logger::getInstance().log(LogLevel::Error, "Failed to create AESCipher: " + std::string(ex.what()));
        response->setStatusCode(HttpResponseStatus::BAD_REQUEST);
        response->setBody("Failed to create payload");
    }
    
    EVP_cleanup();
    
    return response;
}
