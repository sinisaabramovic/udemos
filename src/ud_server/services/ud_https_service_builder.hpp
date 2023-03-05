#ifndef ud_https_service_builder_hpp
#define ud_https_service_builder_hpp

#include "ud_service_builder.hpp"
#include "ud_https_service.hpp"

class ud_https_service_builder : public ud_service_builder<ud_https_service>
{
public:
    ud_https_service_builder() : m_service(nullptr) {}
    void create_service() override
    {
        int port = 8081;
        this->m_service = std::make_unique<ud_https_service>(port, "0.0.0.0");
    }
    std::unique_ptr<ud_service<ud_https_service>> get_service() override
    {
        return std::move(this->m_service);
    }

private:
    std::unique_ptr<ud_service<ud_https_service>> m_service;
};

#endif