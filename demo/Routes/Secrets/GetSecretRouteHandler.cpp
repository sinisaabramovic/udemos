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

#include "../../../src/Protocol/HttpModels/HttpResponse.hpp"
#include "../../../src/Protocol/HttpModels/HttpRequest.hpp"
#include "GetSecretRouteHandler.hpp"
#include "../../../src/Crypto/AESCipher.hpp"
#include "../../../src/Core/Logger/Logger.hpp"

#include <iostream>
#include <nlohmann/json.hpp>
#include "Models/SecretMessage.hpp"

GetSecretRouteHandler::GetSecretRouteHandler(const std::string &method, const std::string &path) : RouteHandler(method, path) {}

std::shared_ptr<HttpResponse> GetSecretRouteHandler::handleRequest(const HttpRequest &request)
{

    auto response = std::make_shared<HttpResponse>();
    response->setStatusCode(HttpResponseStatus::OK);
    response->addHeader("Content-Type", "application/json");

    srand(time(NULL));
    int randomId = rand() % 40 + 1;

    std::string payload = getJoke(randomId);

    OpenSSL_add_all_algorithms();

    SecretMessage secretMessage(payload);
    json secretMessageJson = secretMessage;

    const std::string plaintext = secretMessageJson.dump();
    try
    {
        AESCipher cipher("usr/share/keys/private_key.pem");
        auto responsePayload = cipher.encrypt(reinterpret_cast<const byte *>(plaintext.data()), plaintext.size());
        std::stringstream chiperHexString;
        for (const auto &byte : responsePayload)
        {
            chiperHexString << std::hex << static_cast<int>(byte);
        }

        auto decrypted64RequestBody = cipher.base64Decode(request.getBody());
        auto decryptedData = cipher.decrypt(decrypted64RequestBody.data(), decrypted64RequestBody.size());
        std::string decryptedText(reinterpret_cast<char *>(decryptedData.data()), decryptedData.size());
        Logger::getInstance().log(LogLevel::Info, "Decrypted request data: " + decryptedText);

        auto encodedPayload = cipher.base64Encode(responsePayload);
        response->setStatusCode(HttpResponseStatus::OK);
        response->setBody(encodedPayload);
    }
    catch (const std::exception &ex)
    {
        Logger::getInstance().log(LogLevel::Error, "Failed in AESCipher: " + std::string(ex.what()));
        response->setStatusCode(HttpResponseStatus::BAD_REQUEST);
        response->setBody("Failed to create payload");
    }

    EVP_cleanup();

    return response;
}

std::string GetSecretRouteHandler::getJoke(int id) const
{
    std::string jokes[] = {
        "I told my wife she was drawing her eyebrows too high. She looked surprised.",
        "Why did the tomato turn red? Because it saw the salad dressing!",
        "I'm reading a book on anti-gravity. It's impossible to put down!",
        "I'm reading a book about teleportation. It's bound to take me somewhere!",
        "What did the grape say when it got stepped on? Nothing, it just let out a little whine.",
        "Why do we tell actors to \"break a leg?\" Because every play has a cast!",
        "I used to play piano by ear, but now I use my hands.",
        "I'm a big fan of whiteboards. They're re-markable!",
        "I told my wife she was wearing too much eye makeup. She looked at me with surprise.",
        "I'm on a whiskey diet. I've lost three days already.",
        "I told my wife she was shrinking. She said, \"No, I'm just getting a little bolder!\"",
        "I don't trust people who do acupuncture. They're back stabbers!",
        "Why don't scientists trust atoms? Because they make up everything.",
        "Why did the hipster burn his tongue? He drank his coffee before it was cool.",
        "I'm reading a book on the history of glue. I just can't seem to put it down!",
        "What did one hat say to the other? You stay here, I'll go on ahead!",
        "I used to be indecisive, but now I'm not sure.",
        "Why did the scarecrow win an award? Because he was outstanding in his field!",
        "Why did the math book look sad? Because it had too many problems.",
        "I used to be a baker, but I couldn't make enough dough.",
        "Why did the chicken cross the playground? To get to the other slide.",
        "I was going to tell a joke about pizza, but it's a little cheesy.",
        "Why do birds fly south for the winter? Because it's too far to walk.",
        "I'm reading a book about gravity. It's a heavy subject.",
        "Why don't oysters give to charity? They're shellfish.",
        "I'm reading a book about teleportation. It's bound to have its ups and downs.",
        "What did the janitor say when he jumped out of the closet? \"Supplies!\"",
        "Why did the chicken go to the seance? To talk to the other side.",
        "Why do golfers wear two pairs of pants? In case they get a hole in one!",
        "I'm reading a book about anti-gravity. It's impossible to put down!",
        "Why don't scientists trust atoms? Because they make up everything!",
        "Why don't scientists trust atoms? Because they make up everything!",
        "Why did the man put his money in the freezer? He wanted cold hard cash!",
        "What did the grape say when it got stepped on? Nothing, it just let out a little whine.",
        "I'm on a whiskey diet. I've lost three days already.",
        "I'm reading a book on the history of glue. I just can't seem to put it down!",
        "Why don't scientists trust atoms? Because they make up everything.",
        "I told my wife she was drawing her eyebrows too high. She looked surprised.",
        "I told my wife she was wearing too much eye makeup. She looked at me with surprise.",
        "I'm a big fan of whiteboards. They're re-markable!",
        "I used to play piano by ear, but now I use my hands.",
        "Why did the tomato turn red? Because it saw the salad dressing!",
        "I'm reading a book about gravity. It's a heavy subject.",
        "I'm a huge fan of wind turbines. I think they're absolutely fan-tastic!",
        "I was wondering why the ball was getting bigger. Then it hit me.",
        "I'm reading a book on the history of glue. I just can't seem to put it down!"            
    };

    return jokes[id];
}
