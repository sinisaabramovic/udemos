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
#include <thread>
#include <chrono>
#include <atomic>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    std::string requestHeaders = "";
    try {
        requestHeaders = readRequest(connection);
    } catch (const std::exception& ex) {
        Logger::getInstance().log(LogLevel::Error, "Failed to handleRequest: " + std::string(ex.what()));
        throw std::runtime_error(std::string(ex.what()));
    }
    
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
    } else {
        std::shared_ptr<HttpResponse> response;
        response = std::make_shared<HttpResponse>(HttpResponseStatus::BAD_GATEWAY, "text/plain", "Not Found");
        sendResponse(connection, response->toString());
    }
}

#ifdef __linux__
int HttpProtocolHandler::waitForSocketRead(int epoll_fd, int socket_fd) {
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = socket_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev);
    
    epoll_event events[1];
    int nfds = epoll_wait(epoll_fd, events, 1, 1000);
    
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, nullptr);
    
    return nfds;
}

std::string HttpProtocolHandler::readSocketData(int socket_fd, int bufferSize) {
    std::vector<char> buffer(bufferSize);
    ssize_t bytesRead = read(socket_fd, buffer.data(), buffer.size());
    if (bytesRead <= 0) {
        throw std::runtime_error("Failed to read connection");
    }
    return std::string(buffer.data(), bytesRead);
}

#elif __APPLE__

int HttpProtocolHandler::waitForSocketRead(int kq, int socket_fd) {
    struct kevent evSet;
    struct kevent evList[1];
    
    EV_SET(&evSet, socket_fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    int nevents = kevent(kq, &evSet, 1, evList, 1, nullptr);
    
    EV_SET(&evSet, socket_fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
    kevent(kq, &evSet, 1, nullptr, 0, nullptr);
    
    return nevents;
}

std::string HttpProtocolHandler::readSocketData(Socket& socket_fd, int bufferSize) {
    std::vector<char> buffer(bufferSize);
    ssize_t bytesRead = read(socket_fd.fd(), buffer.data(), buffer.size());
    if (bytesRead <= 0) {
        throw std::runtime_error("Failed to read connection");
    }
    return std::string(buffer.data(), bytesRead);
}
#endif

void HttpProtocolHandler::setSocketNonBlocking(Socket& socket) {
    fcntl(socket.fd(), F_SETFL, O_NONBLOCK);
}

std::string HttpProtocolHandler::readRequestFromSocket(Socket& socket, int bufferSize) {
#ifdef __linux__
    int epoll_fd = epoll_create1(0);
    setSocketNonBlocking(socket);
    int nfds = waitForSocketRead(epoll_fd, socket.fd());
    close(epoll_fd);
#elif __APPLE__
    int kq = kqueue();
    setSocketNonBlocking(socket);
    int nevents = waitForSocketRead(kq, socket.fd());
    close(kq);
#endif
    
#ifdef __linux__
    if (nfds == -1) {
#elif __APPLE__
        if (nevents == -1) {
#endif
            throw std::runtime_error("Failed to read connection");
        }
#ifdef __linux__
        else if (nfds == 0) {
#elif __APPLE__
            else if (nevents == 0) {
#endif
                throw std::runtime_error("Timeout reached");
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            std::string data = readSocketData(socket, bufferSize);
            return data;
        }
        
        std::string HttpProtocolHandler::readRequest(Connection& connection) {
            const int bufferSize = 4096;
            Socket& socket = connection.socket();
            std::string requestHeaders = readRequestFromSocket(socket, bufferSize);
            return requestHeaders;
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
                struct timespec timeout = {5, 0};
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
