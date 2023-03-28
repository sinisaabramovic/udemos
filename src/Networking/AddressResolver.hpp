//
//  AddressResolver.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef AddressResolver_hpp
#define AddressResolver_hpp

#include <string>
#include <vector>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdexcept>

class AddressResolver {
public:
    static std::vector<std::string> resolve(const std::string& hostname);
    
private:
    AddressResolver() = delete;
};

#endif /* AddressResolver_hpp */
