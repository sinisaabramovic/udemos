//
//  RouteHandler.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#include <stdio.h>
#include "RouteHandler.hpp"

RouteHandler::RouteHandler(const std::string& method, const std::string& path) : method_(method), path_(path) {}

const std::string& RouteHandler::getMethod() const {
    return method_;
}

const std::string& RouteHandler::getPath() const {
    return path_;
}
