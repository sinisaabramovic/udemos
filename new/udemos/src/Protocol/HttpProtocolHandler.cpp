//
//  HttpProtocolHandler.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

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
            response = std::make_shared<HttpResponse>(HttpResponseStatus::NOT_FOUND, "text/plain", "Not Found");
        }

        sendResponse(connection, response->toString());
    }
}

std::string HttpProtocolHandler::readRequest(Connection& connection) {
    const int bufferSize = 4096;
    char buffer[bufferSize];
    std::string requestHeaders;

    fcntl(connection.socket().fd(), F_SETFL, O_NONBLOCK);

    fd_set read_fds;
    timeval timeout;
    timeout.tv_sec = 30;
    timeout.tv_usec = 0;

    while (requestHeaders.find("\r\n\r\n") == std::string::npos) {
        FD_ZERO(&read_fds);
        FD_SET(connection.socket().fd(), &read_fds);

        int nfds = select(connection.socket().fd() + 1, &read_fds, nullptr, nullptr, &timeout);
        if (nfds <= 0) {
            break;
        }

        ssize_t bytesRead = read(connection.socket().fd(), buffer, bufferSize - 1);
        if (bytesRead <= 0) {
            break;
        }

        buffer[bytesRead] = '\0';
        requestHeaders += buffer;
    }

    return requestHeaders;
}

void HttpProtocolHandler::sendResponse(Connection& connection, const std::string& response) {
    size_t bytesSent = 0;

    fcntl(connection.socket().fd(), F_SETFL, O_NONBLOCK);

    fd_set write_fds;
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    while (bytesSent < response.length()) {
        FD_ZERO(&write_fds);
        FD_SET(connection.socket().fd(), &write_fds);

        int nfds = select(connection.socket().fd() + 1, nullptr, &write_fds, nullptr, &timeout);
        if (nfds <= 0) {
            break;
        }

        ssize_t sent = write(connection.socket().fd(), response.c_str() + bytesSent, response.length() - bytesSent);
        if (sent <= 0) {
            break;
        }

        bytesSent += sent;
    }

    connection.socket().close();
}
