//
//  HttpResponse.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#include <stdio.h>
#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpResponseStatus code, const std::string &contentType, const std::string &body)
: mStatusCode(code), mContentType(contentType), mBody(body) {}

HttpResponse::HttpResponse() : mStatusCode(HttpResponseStatus::OK), mContentType("text/plain"), mBody("") {}

HttpResponseStatus HttpResponse::getStatusCode() const
{
    return this->mStatusCode;
}

void HttpResponse::setStatusCode(HttpResponseStatus statusCode)
{
    this->mStatusCode = statusCode;
}

std::string HttpResponse::getContentType() const
{
    return this->mContentType;
}

void HttpResponse::setContentType(const std::string &contentType)
{
    this->mContentType = contentType;
}

std::string HttpResponse::getBody() const
{
    return this->mBody;
}

void HttpResponse::setBody(const std::string &body)
{
    this->mBody = body;
}

std::unordered_map<std::string, std::string> HttpResponse::getHeaders() const
{
    return mHeaders;
}

void HttpResponse::setHeaders(std::unordered_map<std::string, std::string> headers)
{
    for (const auto &[headerName, headerValue] : headers)
    {
        this->addHeader(headerName, headerValue);
    }
}

void HttpResponse::addHeader(const std::string &name, const std::string &value)
{
    this->mHeaders[name] = value;
}

std::string HttpResponse::toString()
{
    // TODO: refactor this - currently we set default headers
    for (const auto &[headerName, headerValue] : HttpResponseUtils::corsHeadersDefault())
    {
        this->addHeader(headerName, headerValue);
    }
    //
    
    std::string statusDescription = getStatusDescription(mStatusCode);
    std::string response = "HTTP/1.1 " + std::to_string(static_cast<int>(mStatusCode)) + " " + statusDescription + "\r\n";
    response += "Content-Length: " + std::to_string(mBody.length()) + "\r\n";
    for (auto &[name, value] : mHeaders)
    {
        response += name + ": " + value + "\r\n";
    }
    response += "\r\n" + mBody;
    return response;
}

std::string HttpResponse::getStatusDescription(HttpResponseStatus code) const
{
    switch (code)
    {
        case HttpResponseStatus::OK:
            return "OK";
        case HttpResponseStatus::CREATED:
            return "Created";
        case HttpResponseStatus::ACCEPTED:
            return "Accepted";
        case HttpResponseStatus::NO_CONTENT:
            return "No Content";
        case HttpResponseStatus::MOVED_PERMANENTLY:
            return "Moved Permanently";
        case HttpResponseStatus::FOUND:
            return "Found";
        case HttpResponseStatus::SEE_OTHER:
            return "See Other";
        case HttpResponseStatus::NOT_MODIFIED:
            return "Not Modified";
        case HttpResponseStatus::TEMPORARY_REDIRECT:
            return "Temporary Redirect";
        case HttpResponseStatus::PERMANENT_REDIRECT:
            return "Permanent Redirect";
        case HttpResponseStatus::BAD_REQUEST:
            return "Bad Request";
        case HttpResponseStatus::UNAUTHORIZED:
            return "Unauthorized";
        case HttpResponseStatus::FORBIDDEN:
            return "Forbidden";
        case HttpResponseStatus::NOT_FOUND:
            return "Not Found";
        case HttpResponseStatus::METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case HttpResponseStatus::INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        case HttpResponseStatus::NOT_IMPLEMENTED:
            return "Not Implemented";
        case HttpResponseStatus::BAD_GATEWAY:
            return "Bad Gateway";
        case HttpResponseStatus::SERVICE_UNAVAILABLE:
            return "Service Unavailable";
        case HttpResponseStatus::GATEWAY_TIMEOUT:
            return "Gateway Timeout";
        case HttpResponseStatus::HTTP_VERSION_NOT_SUPPORTED:
            return "HTTP Version Not Supported";
        default:
            return "";
    }
}
