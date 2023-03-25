//
//  RouteMatcher.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#ifndef RouteMatcher_hpp
#define RouteMatcher_hpp

#include <stdio.h>
#include <string>

class RouteMatcher {
public:
    virtual ~RouteMatcher() = default;
    virtual bool matches(const std::string& method, const std::string& path) const = 0;
};

#endif /* RouteMatcher_hpp */
