#ifndef ud_service_factory_hpp
#define ud_service_factory_hpp

#include "services/ud_service_builder.hpp"

template <typename Protocol>
class ud_service_factory
{
public:
    virtual ~ud_service_factory() {}
    virtual std::unique_ptr<ud_service_builder<Protocol>> create_builder() = 0;
};

#endif