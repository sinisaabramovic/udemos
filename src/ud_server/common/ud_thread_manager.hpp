#ifndef ud_thread_manager_hpp
#define ud_thread_manager_hpp

#include <thread>
#include <chrono>
#include <atomic>

class ud_thread_manager
{
public:
    template <typename Func>
    void start(Func &&f)
    {
        std::thread t(std::forward<Func>(f));
        t.detach();
    }
};

#endif
