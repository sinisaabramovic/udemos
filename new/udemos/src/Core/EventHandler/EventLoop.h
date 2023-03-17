//
//  EventLoop.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef EventLoop_hpp
#define EventLoop_hpp

#include <atomic>
#include <thread>
#include "EventQueue.h"

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    
    void run();
    void stop();
    void addEvent(EventPtr event);
    void setProcessEventCallback(std::function<void(EventPtr)> callback) {
        process_event_callback_ = std::move(callback);
    }
    
private:
    void processEvent(EventPtr event);
    
    std::function<void(EventPtr)> process_event_callback_;
    
    std::atomic<bool> running_;
    std::thread event_loop_thread_;
    EventQueue event_queue_;
};

#endif /* EventLoop_hpp */
