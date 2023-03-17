//
//  HttpProtocolHandler.h
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef HttpProtocolHandler_h
#define HttpProtocolHandler_h

#include "ProtocolHandler.h"

class HttpProtocolHandler : public ProtocolHandler {
public:
    void handleRequest(Connection& connection) override;
};

#endif /* HttpProtocolHandler_h */
