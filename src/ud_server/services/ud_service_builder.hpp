#ifndef ud_service_builder_hpp
#define ud_service_builder_hpp

#include "../ud_service.hpp"

template <typename Protocol>
class ud_service_builder
{
public:
    virtual ~ud_service_builder() {}
    virtual void create_service() = 0;
    virtual std::unique_ptr<ud_service<Protocol>> get_service() = 0;
};

#endif