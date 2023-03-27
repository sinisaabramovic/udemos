//
//  HttpRequest.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#include <stdio.h>
#include "HttpRequest.hpp"
#include "../../Core/Logger/Logger.hpp"
#include "../../Core/Utils/StringUtils.hpp"
#include <zlib.h>

HttpRequest::HttpRequest(const std::string &request)
{
    this->parseHttpRequest(request);
}

HttpRequest::HttpRequest() {}

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
        
        // Check for compressed content
        bool compressed = false;
        if (headers.count("Content-Encoding"))
        {
            std::string encoding = headers["Content-Encoding"];
            if (encoding.find("gzip") != std::string::npos || encoding.find("deflate") != std::string::npos)
            {
                compressed = true;
            }
        }
        
        // Extract body from request string
        size_t bodyStartPos = request.find("\r\n\r\n");
        if (bodyStartPos != std::string::npos)
        {
            body = request.substr(bodyStartPos + 4);
        }
        
        // Decompress body if necessary
        if (compressed)
        {
            // Allocate memory for decompressed data
            const size_t BUFFER_SIZE = 4096;
            unsigned char buffer[BUFFER_SIZE];
            std::string decompressedBody;
            
            // Create zlib decompression context
            z_stream stream;
            stream.zalloc = Z_NULL;
            stream.zfree = Z_NULL;
            stream.opaque = Z_NULL;
            stream.avail_in = 0;
            stream.next_in = Z_NULL;
            int ret = inflateInit2(&stream, MAX_WBITS + 16);
            
            // Decompress data
            stream.avail_in = body.size();
            stream.next_in = (Bytef*) body.data();
            do {
                stream.avail_out = BUFFER_SIZE;
                stream.next_out = buffer;
                ret = inflate(&stream, Z_NO_FLUSH);
                if (ret == Z_OK || ret == Z_STREAM_END)
                {
                    decompressedBody.append((char*) buffer, BUFFER_SIZE - stream.avail_out);
                }
            } while (ret == Z_OK);
            
            // Clean up zlib context
            inflateEnd(&stream);
            
            body = decompressedBody;
        }
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
    
    this->handleMultipartFormData();
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

bool HttpRequest::keepAlive() const {
    std::string connection_value = getHeaderValue("Connection");
    return !connection_value.empty() && StringUtils::trim(connection_value) == "keep-alive";
}

std::string HttpRequest::getHeaderValue(const std::string& key) const {
    std::string lowercase_key = key;
    std::transform(lowercase_key.begin(), lowercase_key.end(), lowercase_key.begin(), ::tolower);
    
    for (const auto& header : mHeaders) {
        std::string header_key = header.first;
        std::transform(header_key.begin(), header_key.end(), header_key.begin(), ::tolower);
        
        if (header_key == lowercase_key) {
            return header.second;
        }
    }
    
    return ""; // Return an empty string if the key is not found
}
