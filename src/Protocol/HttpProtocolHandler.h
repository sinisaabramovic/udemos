//
//  HttpProtocolHandler.h
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef HttpProtocolHandler_h
#define HttpProtocolHandler_h

#include <future>
#include "ProtocolHandler.h"

class HttpProtocolHandler : public ProtocolHandler
{
public:
    void handleRequest(Connection &connection) override;

private:
    std::string readRequest(Connection &connection);
    void sendResponse(Connection &connection, const std::string &response);
    void setSocketNonBlocking(Socket &socket);
    int waitForSocketRead(int kq, int socket_fd);
#ifdef __APPLE__
    std::string readSocketData(Socket &socket, int bufferSize);
#elif __linux__
    std::string readSocketData(int socket_fd, int bufferSize);
#endif
    std::string readRequestFromSocket(Socket &socket, int bufferSize);
};

#endif /* HttpProtocolHandler_h */
