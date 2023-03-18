//
//  EventQueue.h
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef EventQueue_h
#define EventQueue_h

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Event.h"


class EventQueue {
public:
    void push(EventPtr event);
    EventPtr pop();
    bool empty() const;

private:
    std::queue<EventPtr> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
};

#endif /* EventQueue_h */
