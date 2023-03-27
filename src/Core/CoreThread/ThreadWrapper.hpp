//
//  ThreadWrapper.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef ThreadWrapper_hpp
#define ThreadWrapper_hpp

#include <thread>

class ThreadWrapper {
public:
    template <typename Function, typename... Args>
    explicit ThreadWrapper(Function&& func, Args&&... args)
    : thread_(std::forward<Function>(func), std::forward<Args>(args)...) {}
    
    ~ThreadWrapper() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    ThreadWrapper(const ThreadWrapper&) = delete;
    ThreadWrapper& operator=(const ThreadWrapper&) = delete;
    
private:
    std::thread thread_;
};

#endif /* ThreadWrapper_hpp */
