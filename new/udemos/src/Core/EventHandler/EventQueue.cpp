//
//  EventQueue.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "EventQueue.h"

void EventQueue::push(EventPtr event) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(event);
    lock.unlock();
    cond_var_.notify_one();
}

EventPtr EventQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_var_.wait(lock, [this]() { return !queue_.empty(); });
    EventPtr event = queue_.front();
    queue_.pop();
    return event;
}

bool EventQueue::empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
}
