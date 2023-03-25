//
//  ServiceFactory.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef ServiceFactory_hpp
#define ServiceFactory_hpp

#include "HttpService.hpp"
#include "Configuration.hpp"
#include <memory>

class ServiceFactory {
public:
    static std::unique_ptr<HttpService> createService(const Configuration& config);
};

#endif /* ServiceFactory_hpp */
