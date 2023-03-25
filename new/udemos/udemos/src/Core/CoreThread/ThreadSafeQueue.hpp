//
//  ThreadSafeQueue.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef ThreadSafeQueue_hpp
#define ThreadSafeQueue_hpp

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
public:
    void push(const T& value);
    bool try_pop(T& value);
    bool empty() const;

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond_var_;
};

template <typename T>
void ThreadSafeQueue<T>::push(const T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(value);
    lock.unlock();
    cond_var_.notify_one();
}

template <typename T>
bool ThreadSafeQueue<T>::try_pop(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    value = queue_.front();
    queue_.pop();
    return true;
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
}

#endif /* ThreadSafeQueue_hpp */
