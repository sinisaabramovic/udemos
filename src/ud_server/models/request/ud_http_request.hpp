#ifndef ud_http_request_hpp
#define ud_http_request_hpp

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

class ud_http_request
{
private:
    std::string m_method;
    std::string m_path;
    std::unordered_map<std::string, std::string> m_headers;
    std::string m_body;
    std::string m_queryString;
    std::unordered_map<std::string, std::string> m_params;
    std::unordered_map<std::string, std::string> m_cookies;
    std::string m_authToken;
    std::unordered_map<std::string, std::string> m_formFields;
    std::unordered_map<std::string, std::string> m_fileUploads;

    void parse_http_request(const std::string &request);
    void handle_multipart_form_data();
    bool is_valid_json(const std::string &jsonStr) const;
    
public:
    ud_http_request(const std::string &request);
    ~ud_http_request();

    std::string get_path() const 
    {
        return this->m_path;
    }
    
    std::string get_method() const 
    {
        return this->m_method;
    }

    std::string get_body() const 
    {
        return this->m_body;
    }
};

ud_http_request::ud_http_request(const std::string &request)
{
    this->parse_http_request(request);
    this->handle_multipart_form_data();
}

ud_http_request::~ud_http_request()
{
}

void ud_http_request::parse_http_request(const std::string &request)
{
    std::istringstream iss(request);
    std::string line;

    // Parse the first line of the request (e.g., "GET / HTTP/1.1")
    std::getline(iss, line);
    std::istringstream firstLineIss(line);
    std::string method, path, httpVersion;
    firstLineIss >> method >> path >> httpVersion;

    // Parse the headers of the request
    std::unordered_map<std::string, std::string> headers;
    while (std::getline(iss, line) && line != "\r")
    {
        std::istringstream headerIss(line);
        std::string headerName, headerValue;
        std::getline(headerIss, headerName, ':');
        std::getline(headerIss, headerValue);
        headers[headerName] = headerValue;
    }

    // Parse the query string (if any)
    std::string queryString;
    size_t queryStringStart = path.find('?');
    if (queryStringStart != std::string::npos)
    {
        queryString = path.substr(queryStringStart + 1);
        path = path.substr(0, queryStringStart);
    }

    // Parse the URL parameters (if any)
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

    // Parse the body of the request (if any)
    std::string body;
    if (headers.count("Content-Length"))
    {
        int contentLength = std::stoi(headers["Content-Length"]);
        body = request.substr(request.size() - contentLength);
    }

    // Extract the authentication token from the Authorization header (if present)
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

    // Extract cookies from the Cookie header (if present)
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
                cookies[cookieName] = cookieValue;
            }
        }
    }

    // Store the parsed values in the object's member variables
    this->m_method = method;
    this->m_path = path;
    this->m_headers = headers;
    this->m_body = body;
    this->m_authToken = authToken;
    this->m_cookies = cookies;
}

void ud_http_request::handle_multipart_form_data()
{
    if (m_headers.count("Content-Type") && m_headers["Content-Type"].find("multipart/form-data") != std::string::npos)
    {
        std::string boundary = "--" + m_headers["Content-Type"].substr(m_headers["Content-Type"].find("boundary=") + 9);

        std::vector<std::string> parts;
        size_t pos = 0;
        while ((pos = m_body.find(boundary, pos)) != std::string::npos)
        {
            parts.push_back(m_body.substr(pos + boundary.length(), m_body.find(boundary, pos + boundary.length()) - pos - boundary.length() - 2));
            pos += boundary.length() + parts.back().length() + 4;
        }

        std::unordered_map<std::string, std::string> formFields;
        std::unordered_map<std::string, std::string> fileUploads;
        for (const auto &part : parts)
        {
            std::string partBody = part.substr(part.find("\r\n\r\n") + 4);
            std::istringstream partIss(part.substr(0, part.find("\r\n\r\n")));

            std::string headersLine;
            while (std::getline(partIss, headersLine, '\r'))
            {
            }

            std::string fieldName = headersLine.substr(headersLine.find("name=\"") + 6);
            fieldName = fieldName.substr(0, fieldName.find("\""));

            if (headersLine.find("filename=\"") != std::string::npos)
            {
                std::string fileName = headersLine.substr(headersLine.find("filename=\"") + 10);
                fileName = fileName.substr(0, fileName.find("\""));

                fileUploads[fieldName] = partBody;
                this->m_fileUploads[fileName] = partBody;
            }
            else
            {
                formFields[fieldName] = partBody;
            }
        }

        this->m_formFields = formFields;
    }
}

bool ud_http_request::is_valid_json(const std::string &json_string) const
{
    rapidjson::Document document;
    if (document.Parse(json_string.c_str()).HasParseError())
    {
        // Parsing error occurred
        std::cout << "JSON parse error: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
        return false;
    }

    // Successfully parsed as JSON
    return true;
}


#endif