//
//  HttpRequest.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef HttpRequest_h
#define HttpRequest_h

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

class HttpRequest
{
public:
    HttpRequest(const std::string &request);
    HttpRequest();
    ~HttpRequest();
    
    std::string getPath() const
    {
        return this->mPath;
    }
    
    std::string getMethod() const
    {
        return this->mMethod;
    }
    
    std::string getBody() const
    {
        return this->mBody;
    }
    
    std::unordered_map<std::string, std::string> getCookies() const {
        return this->mCookies;
    }
    
    std::unordered_map<std::string, std::string> getFormFields() const {
        return this->mFormFields;
    }
    
    std::unordered_map<std::string, std::string> getFileUploads() const {
        return this->mFileUploads;
    }
    
    std::unordered_map<std::string, std::string> getHeaders() const {
        return this->mHeaders;
    }
    
    bool keepAlive() const;
    
    void parseHttpRequest(const std::string &request);
    
    bool isValidJson(const std::string &jsonStr) const;
    
private:
    std::string mMethod;
    std::string mPath;
    std::unordered_map<std::string, std::string> mHeaders;
    std::string mBody;
    std::string mQueryString;
    std::unordered_map<std::string, std::string> mParams;
    std::unordered_map<std::string, std::string> mCookies;
    std::string mAuthToken;
    std::unordered_map<std::string, std::string> mFormFields;
    std::unordered_map<std::string, std::string> mFileUploads;
        
    void handleMultipartFormData();
    std::string getHeaderValue(const std::string& key) const;
    
};

#endif /* HttpRequest_h */