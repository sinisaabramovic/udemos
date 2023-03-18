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
#include "HttpProtocolHandler.h"
#include "Connection.hpp"
#include "RouteHandlerFactory.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

void HttpProtocolHandler::handleRequest(Connection& connection) {
    std::string requestHeaders = readRequest(connection);
    if (!requestHeaders.empty()) {
        std::cout << "Received request:\n" << requestHeaders << std::endl;

        // Parse the raw request string into an HttpRequest object
        HttpRequest request({requestHeaders});

        // Find the appropriate route handler
        auto routeHandler = RouteHandlerFactory::getInstance().getHandler(request.getMethod(), request.getPath());

        // If a route handler is found, use it to generate an HttpResponse object
        std::shared_ptr<HttpResponse> response;
        if (routeHandler) {
            response = routeHandler->handleRequest(request);
        } else {
            // If no route handler is found, generate a 404 Not Found response
            response = std::make_shared<HttpResponse>(HttpResponseStatus::NOT_FOUND, "text/plain", "Not Found");
        }

        // Serialize the HttpResponse object and send it to the client
        sendResponse(connection, response->toString());
    }
}

std::string HttpProtocolHandler::readRequest(Connection& connection) {
    // Set the socket to non-blocking mode.
    int flags = fcntl(connection.socket().fd(), F_GETFL, 0);
    fcntl(connection.socket().fd(), F_SETFL, flags | O_NONBLOCK);
    
    const int bufferSize = 4096;
    char buffer[bufferSize];
    int bytesRead = 0;
    std::string requestHeaders;
    
    // Read data until we find a double CRLF, which indicates the end of the headers.
    while (requestHeaders.find("\r\n\r\n") == std::string::npos) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(connection.socket().fd(), &read_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int nfds = select(connection.socket().fd() + 1, &read_fds, nullptr, nullptr, &timeout);
        if (nfds < 0) {
            std::cerr << "select() error: " << strerror(errno) << std::endl;
            break;
        } else if (nfds == 0) {
            std::cerr << "readRequest timed out." << std::endl;
            break;
        } else {
            bytesRead = read(connection.socket().fd(), buffer, bufferSize - 1);
            if (bytesRead < 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    std::cerr << "Failed to read request: " << strerror(errno) << std::endl;
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
    // Set the socket to non-blocking mode.
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
            std::cerr << "select() error: " << strerror(errno) << std::endl;
            break;
        } else if (nfds == 0) {
            std::cerr << "sendResponse timed out." << std::endl;
            break;
        } else {
            int sent = write(connection.socket().fd(), response.c_str() + bytesSent, response.length() - bytesSent);
            if (sent < 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    std::cerr << "Failed to send response: " << strerror(errno) << std::endl;
                    break;
                }
            } else {
                bytesSent += sent;
            }
        }
    }
    
    // Close the connection.
    connection.socket().close();
}

std::string HttpProtocolHandler::generateResponse(const std::string& requestHeaders) {
    // In this example, we'll just generate a simple HTTP 200 OK response.
    // You can modify this function to parse the request headers and generate
    // a more appropriate response based on the request.
    
    std::string response = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 2\r\n"
    "\r\n"
    "OK";
    return response;
}

