//
//  HttpRequest.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#include <stdio.h>
#include "HttpRequest.hpp"
#include "Logger.hpp"

HttpRequest::HttpRequest(const std::string &request)
{
    this->parseHttpRequest(request);
    this->handleMultipartFormData();
}

HttpRequest::~HttpRequest() {}

void HttpRequest::parseHttpRequest(const std::string &request)
{
    std::istringstream iss(request);
    std::string line;
    
    std::getline(iss, line);
    std::istringstream firstLineIss(line);
    std::string method, path, httpVersion;
    firstLineIss >> method >> path >> httpVersion;
    
    std::unordered_map<std::string, std::string> headers;
    while (std::getline(iss, line) && line != "\r")
    {
        std::istringstream headerIss(line);
        std::string headerName, headerValue;
        std::getline(headerIss, headerName, ':');
        std::getline(headerIss, headerValue);
        headers[headerName] = headerValue;
    }
    
    std::string queryString;
    size_t queryStringStart = path.find('?');
    if (queryStringStart != std::string::npos)
    {
        queryString = path.substr(queryStringStart + 1);
        path = path.substr(0, queryStringStart);
    }
    
    std::unordered_map<std::string, std::string> params;
    std::istringstream paramIss(queryString);
    std::string paramPair;
    while (std::getline(paramIss, paramPair, '&'))
    {
        std::istringstream keyValueIss(paramPair);
        std::string key, value;
        std::getline(keyValueIss, key, '=');
        std::getline(keyValueIss, value);
        params[key] = value;
    }
    
    std::string body;
    if (headers.count("Content-Length"))
    {
        int contentLength = std::stoi(headers["Content-Length"]);
        body = request.substr(request.size() - contentLength);
    }
    
    std::string authToken;
    if (headers.count("Authorization"))
    {
        std::string authHeader = headers["Authorization"];
        size_t spacePos = authHeader.find(' ');
        if (spacePos != std::string::npos && authHeader.substr(0, spacePos) == "Bearer")
        {
            authToken = authHeader.substr(spacePos + 1);
        }
    }

    std::unordered_map<std::string, std::string> cookies;
    if (headers.count("Cookie"))
    {
        std::string cookieHeader = headers["Cookie"];
        std::istringstream cookieIss(cookieHeader);
        std::string cookiePair;
        while (std::getline(cookieIss, cookiePair, ';'))
        {
            size_t eqPos = cookiePair.find('=');
            if (eqPos != std::string::npos)
            {
                std::string cookieName = cookiePair.substr(0, eqPos);
                std::string cookieValue = cookiePair.substr(eqPos + 1);
                
                // Trim whitespaces and remove carriage return
                cookieName.erase(0, cookieName.find_first_not_of(" \t"));
                cookieName.erase(cookieName.find_last_not_of(" \t") + 1);
                cookieValue.erase(0, cookieValue.find_first_not_of(" \t"));
                cookieValue.erase(cookieValue.find_last_not_of(" \t\r") + 1);
                
                cookies[cookieName] = cookieValue;
            }
        }
    }
    
    this->mMethod = method;
    this->mPath = path;
    this->mHeaders = headers;
    this->mBody = body;
    this->mAuthToken = authToken;
    this->mCookies = cookies;
}

void HttpRequest::handleMultipartFormData()
{
    if (mHeaders.count("Content-Type") && mHeaders["Content-Type"].find("multipart/form-data") != std::string::npos)
    {
        std::string boundary = "--" + mHeaders["Content-Type"].substr(mHeaders["Content-Type"].find("boundary=") + 9);

        std::vector<std::string> parts;
        size_t pos = 0;
        while ((pos = mBody.find(boundary, pos)) != std::string::npos)
        {
            size_t nextPos = mBody.find(boundary, pos + boundary.length());
            if (nextPos == std::string::npos)
                nextPos = mBody.size();

            parts.push_back(mBody.substr(pos + boundary.length(), nextPos - pos - boundary.length() - 2));
            pos = nextPos;
        }

        std::unordered_map<std::string, std::string> formFields;
        std::unordered_map<std::string, std::string> fileUploads;
        for (const auto &part : parts)
        {
            std::string partBody = part.substr(part.find("\r\n\r\n") + 4);
            size_t endOfBody = partBody.find("\r\n");
            if (endOfBody != std::string::npos)
            {
                partBody = partBody.substr(0, endOfBody);
            }
            std::istringstream partIss(part.substr(0, part.find("\r\n\r\n")));

            std::string headersLine;
            std::getline(partIss, headersLine, '\r');
            partIss.ignore(1, '\n'); // Skip the '\n' after '\r'

            std::string fieldName = headersLine.substr(headersLine.find("name=\"") + 6);
            fieldName = fieldName.substr(0, fieldName.find("\""));

            if (headersLine.find("filename=\"") != std::string::npos)
            {
                std::string fileName = headersLine.substr(headersLine.find("filename=\"") + 10);
                fileName = fileName.substr(0, fileName.find("\""));

                fileUploads[fieldName] = partBody;
                this->mFileUploads[fileName] = partBody;
            }
            else
            {
                formFields[fieldName] = partBody;
            }
        }

        this->mFormFields = formFields;
    }
}

bool HttpRequest::isValidJson(const std::string &jsonStr) const
{
    try
    {
        auto basicJson = nlohmann::json::parse(jsonStr);
        return true;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        Logger::getInstance().log(LogLevel::Info, "JSON parse error: " + std::string(e.what()));
        return false;
    }
    
    return true;
}
