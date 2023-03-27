//
//  HttpResponse.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef HttpResponse_hpp
#define HttpResponse_hpp

#include <string>
#include <utility>
#include <iostream>
#include <map>
#include <unordered_map>
#include "HttpResponseUtils.hpp"

enum class HttpResponseStatus
{
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

class HttpResponse
{
public:
    HttpResponse(HttpResponseStatus code, const std::string &contentType, const std::string &body);
    HttpResponse();
    ~HttpResponse() {}
    
    std::string toString();
    
    HttpResponseStatus getStatusCode() const;
    void setStatusCode(HttpResponseStatus statusCode);
    
    std::string getContentType() const;
    void setContentType(const std::string &contentType);
    
    std::string getBody() const;
    void setBody(const std::string &body);
    
    std::unordered_map<std::string, std::string> getHeaders() const;
    void setHeaders(std::unordered_map<std::string, std::string> headers);
    void addHeader(const std::string &name, const std::string &value);
    
private:
    HttpResponseStatus mStatusCode;
    std::string mContentType;
    std::string mBody;
    std::unordered_map<std::string, std::string> mHeaders;
    std::string getStatusDescription(HttpResponseStatus code) const;
};

#endif // HttpResponse_hpp

