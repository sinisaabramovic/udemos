//
//  HttpProtocolHandler.h
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef HttpProtocolHandler_h
#define HttpProtocolHandler_h

#include <experimental/coroutine>
#include <future>
#include "ProtocolHandler.h"

class HttpProtocolHandler : public ProtocolHandler {
public:
    void handleRequest(Connection& connection) override;
    
private:
    std::string readRequest(Connection& connection);
    void sendResponse(Connection& connection, const std::string& response);
};

#endif /* HttpProtocolHandler_h */
