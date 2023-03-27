//
//  ThreadManager.hpp
//  CommonPod
//
//  Created by Sinisa Abramovic on 14.03.2023..
//

#ifndef ThreadManager_h
#define ThreadManager_h

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <memory>
#include <atomic>
#include <condition_variable>

#include "BaseThread/BaseThread.hpp"

class ThreadManager : public std::enable_shared_from_this<ThreadManager> {
public:
    
    void add_thread(std::shared_ptr<BaseThread> thread) {
        threads_[thread->getThreadId()] = thread;
        thread->set_on_finish_delegate([self = shared_from_this()](int id) {
            self->remove_thread(id);
        });
    }
    
    void remove_thread(int id) {
        auto it = threads_.find(id);
        if (it != threads_.end()) {
            threads_.erase(it);
        }
        num_running_--;
        cv_.notify_one();
    }
    
    void start_thread(int id) {
        auto it = threads_.find(id);
        if (it != threads_.end()) {
            num_running_++;
            it->second->start();
        }
    }
    
    void stop_thread(int id) {
        auto it = threads_.find(id);
        if (it != threads_.end()) {
            it->second->stop();
        }
    }
    
    void join_all() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() {
            return num_running_ == 0;
        });
    }
    
private:
    std::map<int, std::shared_ptr<BaseThread>> threads_;
    std::atomic<int> num_running_{0};
    std::mutex mutex_;
    std::condition_variable cv_;
};

#endif /* ThreadManager_h */
