//
//  ProtocolHandler.h
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef ProtocolHandler_h
#define ProtocolHandler_h

#include <experimental/coroutine>
#include <future>
#include "../Networking/Connection.hpp"

class ProtocolHandler {
public:
    virtual ~ProtocolHandler() = default;
    
    virtual void handleRequest(Connection& connection) = 0;
};

#endif /* ProtocolHandler_h */
