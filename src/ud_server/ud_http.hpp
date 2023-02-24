#ifndef ud_http_hpp
#define ud_http_hpp

#include <thread>
#include <chrono>
#include <atomic>

#include "ud_server.hpp"

class ud_http : public ud_server
{
private:
    /* data */
public:
    ud_http()
    {
        stop_flag = false;
    }
    ~ud_http() {}

    void pause_listen(bool pause)
    {        
        std::unique_lock<std::mutex> lock(pause_mutex);
        pause_flag = pause;
        std::cout << "pause_listen: " << pause << std::endl;
        pause_cv.notify_one();
        stoped = pause;
    }

    void stop_listen() override
    {
        stop_flag = true;
        stoped = true;
    }

    bool is_running() override
    {
        return !stoped;
    }
    
    bool is_paused() override
    {
        return !paused;
    }

    void start_listen(u_int32_t port, const std::string &host_name, status_delegate delegate) override
    {
        using ud_result_type = ud_result<ud_result_success, ud_result_failure>;

        // Start a new thread
        std::thread listener_thread([&]()
        {
            bool success = bind_listener(port, host_name);
            if (!success) 
            {
                ud_result_type failure_listen{ud_result_failure{"Failed to bind listener to port."}};
                delegate(failure_listen);                
                stop_listen();
                return;
            }

            while (!stop_flag) 
            {
                std::unique_lock<std::mutex> lock(pause_mutex);
                pause_cv.wait(lock, [&]() {
                    return pause_flag.load() == false; 
                });
                lock.unlock();

                std::cout << "running \n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                ud_result_type my_result{ud_result_success{"OK"}};
                delegate(my_result);
            } 
        });

        // Detach the thread so that it can continue running in the background
        listener_thread.detach();
    }

private:
    bool bind_listener(u_int32_t port, const std::string &host_name)
    {
        // TODO: Implement code to bind listener to port and host_name
        // Return true if successful, false otherwise
        std::cout << "bind_listener() \n";
        return true;
    }

    void unbind_listener()
    {
        return;
    }
};

#endif