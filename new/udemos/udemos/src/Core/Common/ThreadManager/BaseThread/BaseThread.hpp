//
//  BaseThread.hpp
//  CommonPod
//
//  Created by Sinisa Abramovic on 14.03.2023..
//

#ifndef BaseThread_h
#define BaseThread_h

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <memory>
#include <atomic>
#include <condition_variable>

class BaseThread : public std::enable_shared_from_this<BaseThread> {
public:
    BaseThread(int id) : threadId_(id) {}
    virtual ~BaseThread() {}
    
    void start() {
        if (!running_) {
            running_ = true;
            thread_ = std::thread([self = shared_from_this()]() { self->run(); });
        }
    }
    
    void stop() {
        running_ = false;
    }
    
    void join() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    bool is_running() const {
        return running_;
    }
    
    int32_t getThreadId() const {
        return this->threadId_;
    }
    
    void set_on_finish_delegate(std::function<void(int)> delegate) {
        on_finish_delegate_ = delegate;
    }
    
private:
    virtual void run() = 0;
    
    std::thread thread_;
    bool running_ = false;
    int32_t threadId_;
    
public:
    std::function<void(int)> on_finish_delegate_;
};

#endif /* BaseThread_h */
