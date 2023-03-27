//
//  HttpMethod.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#ifndef HttpMethod_hpp
#define HttpMethod_hpp

#include <string>

class HttpMethod {
public:
    static const std::string GET;
    static const std::string POST;
    static const std::string PUT;
    static const std::string DELETE;
    static const std::string PATCH;
};

#endif /* HttpMethod_hpp */
