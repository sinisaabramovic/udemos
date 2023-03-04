#ifndef ud_https_service_factory_hpp
#define ud_https_service_factory_hpp

#include "ud_service_factory.hpp"
#include "services/ud_https_service.hpp"
#include "services/ud_service_builder.hpp"
#include "services/ud_http_service_builder.hpp"

class ud_https_service_factory : public ud_service_factory<ud_https_service>
{
public:
    std::unique_ptr<ud_service_builder<ud_https_service>> create_builder() override
    {
        return std::make_unique<ud_https_service_builder>();
    }
};

#endif