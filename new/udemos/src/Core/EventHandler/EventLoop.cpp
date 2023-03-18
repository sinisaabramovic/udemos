//
//  EventLoop.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "EventLoop.h"

EventLoop::EventLoop() : running_(false) {}

EventLoop::~EventLoop() {
    stop();
}

void EventLoop::run() {
    running_ = true;
    event_loop_thread_ = std::make_unique<std::thread>([this]() {
        while (running_) {
            EventPtr event = event_queue_.pop();
            if (event != nullptr) {
                processEvent(event);
            }
        }
    });
}

void EventLoop::stop() {
    if (running_) {
        running_ = false;
        event_queue_.push(nullptr);
        event_loop_thread_->join();
        event_loop_thread_.reset(); // Release the thread object
    }
}

void EventLoop::addEvent(EventPtr event) {
    event_queue_.push(event);
}

void EventLoop::processEvent(EventPtr event) {
    if (process_event_callback_) {
        process_event_callback_(event);
    }
}
