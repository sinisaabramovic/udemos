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
#include "../Networking/Connection.hpp"
#include "../Route/RouteHandlerFactory.hpp"
#include "HttpModels/HttpRequest.hpp"
#include "HttpModels/HttpResponse.hpp"
#include "../Core/Logger/Logger.hpp"

#include <sys/types.h>
#ifdef __linux__
#include <sys/epoll.h>
#elif __APPLE__
#include <sys/event.h>
#endif

void HttpProtocolHandler::handleRequest(Connection& connection) {
    HttpRequest request{};
    std::string requestHeaders = readRequest(connection);
    if (!requestHeaders.empty()) {
        Logger::getInstance().log(LogLevel::Info, requestHeaders);
        
        request.parseHttpRequest(requestHeaders);
        
        auto routeHandler = RouteHandlerFactory::getInstance().getHandler(request.getMethod(), request.getPath());
        connection.setKeepAlive(request.keepAlive());
        
        std::shared_ptr<HttpResponse> response;
        if (routeHandler) {
            response = routeHandler->handleRequest(request);
        } else {
            response = std::make_shared<HttpResponse>(HttpResponseStatus::NOT_FOUND, "text/plain", "Not Found");
        }
        
        if (connection.keepAlive()) {
            response->addHeader("Connection", "keep-alive");
        }
        
        sendResponse(connection, response->toString());
        if (!connection.keepAlive()) {
            return;
        }
    }
}

std::string HttpProtocolHandler::readRequest(Connection& connection) {
    const int bufferSize = 4096;
    char buffer[bufferSize];
    std::stringstream requestHeaders;
    
    fcntl(connection.socket().fd(), F_SETFL, O_NONBLOCK);
    
#ifdef __linux__
    int epoll_fd = epoll_create1(0);
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = connection.socket().fd();
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connection.socket().fd(), &ev);
    
    epoll_event events[1];
#elif __APPLE__
    int kq = kqueue();
    struct kevent evSet;
    struct kevent evList[1];
    
    EV_SET(&evSet, connection.socket().fd(), EVFILT_READ, EV_ADD, 0, 0, nullptr);
#endif
    
    while (requestHeaders.str().find("\r\n\r\n") == std::string::npos) {
        int nfds;
        
#ifdef __linux__
        nfds = epoll_wait(epoll_fd, events, 1, 1000);
#elif __APPLE__
        struct timespec timeout = {1, 0};
        nfds = kevent(kq, &evSet, 1, evList, 1, &timeout);
#endif
        
        if (nfds <= 0) {
            break;
        }
        
        ssize_t bytesRead = read(connection.socket().fd(), buffer, bufferSize - 1);
        if (bytesRead <= 0) {
            break;
        }
        
        buffer[bytesRead] = '\0';
        requestHeaders << buffer;
    }
    
#ifdef __linux__
    close(epoll_fd);
#elif __APPLE__
    close(kq);
#endif
    
    return requestHeaders.str();
}

void HttpProtocolHandler::sendResponse(Connection& connection, const std::string& response) {
    size_t bytesSent = 0;
    
    fcntl(connection.socket().fd(), F_SETFL, O_NONBLOCK);
    
#ifdef __linux__
    int epoll_fd = epoll_create1(0);
    epoll_event ev;
    ev.events = EPOLLOUT;
    ev.data.fd = connection.socket().fd();
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connection.socket().fd(), &ev);
    
    epoll_event events[1];
#elif __APPLE__
    int kq = kqueue();
    struct kevent evSet;
    struct kevent evList[1];
    
    EV_SET(&evSet, connection.socket().fd(), EVFILT_WRITE, EV_ADD, 0, 0, nullptr);
#endif
    
    while (bytesSent < response.length()) {
        int nfds;
        
#ifdef __linux__
        nfds = epoll_wait(epoll_fd, events, 1, 1000);
#elif __APPLE__
        struct timespec timeout = {1, 0};
        nfds = kevent(kq, &evSet, 1, evList, 1, &timeout);
#endif
        
        if (nfds <= 0) {
            break;
        }
        
        ssize_t sent = write(connection.socket().fd(), response.c_str() + bytesSent, response.length() - bytesSent);
        if (sent <= 0) {
            break;
        }
        
        bytesSent += sent;
    }
    
#ifdef __linux__
    close(epoll_fd);
#elif __APPLE__
    close(kq);
#endif
    
    connection.socket().close();
}
