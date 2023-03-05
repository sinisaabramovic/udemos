#ifndef ud_service_hpp
#define ud_service_hpp

#include <memory>
#include <string>

template<typename Protocol>
class ud_service
{
public:
    virtual ~ud_service() {}
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool is_running() = 0;
};

#endif