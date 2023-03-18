//
//  HttpProtocolHandler.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <fcntl.h>
#include <sys/select.h>
#include <map>
#include "HttpProtocolHandler.h"
#include "Connection.hpp"
#include "RouteHandlerFactory.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"

void HttpProtocolHandler::handleRequest(Connection& connection) {
    std::string requestHeaders = readRequest(connection);
    if (!requestHeaders.empty()) {
        Logger::getInstance().log(LogLevel::Info, requestHeaders);

        HttpRequest request({requestHeaders});

        auto routeHandler = RouteHandlerFactory::getInstance().getHandler(request.getMethod(), request.getPath());

        std::shared_ptr<HttpResponse> response;
        if (routeHandler) {
            response = routeHandler->handleRequest(request);
        } else {
            // If no route handler is found, generate a 404 Not Found response
            auto headers = request.getHeaders();
            if (!headers.empty() && headers.count("Content-Type")) {
                response = std::make_shared<HttpResponse>(HttpResponseStatus::NOT_FOUND, headers["Content-Type"], "Not Found");
            } else {
                response = std::make_shared<HttpResponse>(HttpResponseStatus::NOT_FOUND, "text/plain", "Not Found");
            }
        }

        sendResponse(connection, response->toString());
    }
}

std::string HttpProtocolHandler::readRequest(Connection& connection) {
    int flags = fcntl(connection.socket().fd(), F_GETFL, 0);
    fcntl(connection.socket().fd(), F_SETFL, flags | O_NONBLOCK);
    
    const int bufferSize = 4096;
    char buffer[bufferSize];
    int bytesRead = 0;
    std::string requestHeaders;
    
    while (requestHeaders.find("\r\n\r\n") == std::string::npos) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(connection.socket().fd(), &read_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int nfds = select(connection.socket().fd() + 1, &read_fds, nullptr, nullptr, &timeout);
        if (nfds < 0) {
            Logger::getInstance().log(LogLevel::Error, "select() error: " + std::string(strerror(errno)));
            break;
        } else if (nfds == 0) {
            Logger::getInstance().log(LogLevel::Error, "readRequest timed out. ");
            break;
        } else {
            bytesRead = read(connection.socket().fd(), buffer, bufferSize - 1);
            if (bytesRead < 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    Logger::getInstance().log(LogLevel::Error, "Failed to read request: " + std::string(strerror(errno)));
                    break;
                }
            } else {
                buffer[bytesRead] = '\0';
                requestHeaders += buffer;
            }
        }
    }
    
    return requestHeaders;
}

void HttpProtocolHandler::sendResponse(Connection& connection, const std::string& response) {
    int flags = fcntl(connection.socket().fd(), F_GETFL, 0);
    fcntl(connection.socket().fd(), F_SETFL, flags | O_NONBLOCK);
    
    size_t bytesSent = 0;
    while (bytesSent < response.length()) {
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(connection.socket().fd(), &write_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int nfds = select(connection.socket().fd() + 1, nullptr, &write_fds, nullptr, &timeout);
        if (nfds < 0) {
            Logger::getInstance().log(LogLevel::Error, "select() error: " + std::string(strerror(errno)));
            break;
        } else if (nfds == 0) {
            Logger::getInstance().log(LogLevel::Error, "sendResponse timed out." + std::string(strerror(errno)));
            break;
        } else {
            int sent = write(connection.socket().fd(), response.c_str() + bytesSent, response.length() - bytesSent);
            if (sent < 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    Logger::getInstance().log(LogLevel::Error, "Failed to send response: " + std::string(strerror(errno)));
                    break;
                }
            } else {
                bytesSent += sent;
            }
        }
    }
    
    connection.socket().close();
}
