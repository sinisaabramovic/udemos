#ifndef ud_http_service_factory_hpp
#define ud_http_service_factory_hpp

#include "../ud_service_factory.hpp"
#include "../services/ud_http_service.hpp"
#include "../services/ud_service_builder.hpp"
#include "../services/ud_http_service_builder.hpp"

class ud_http_service_factory : public ud_service_factory<ud_http_service>
{
public:
    std::unique_ptr<ud_service_builder<ud_http_service>> create_builder() override
    {
        return std::make_unique<ud_http_service_builder>();
    }
};

#endif