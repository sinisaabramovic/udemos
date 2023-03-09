//
//  ud_c_message_producer.hpp
//  common_tester
//
//  Created by Sinisa Abramovic on 09.03.2023..
//

#ifndef ud_c_message_producer_hpp
#define ud_c_message_producer_hpp

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <queue>

#include "ud_c_message_struct.hpp"
#include "ud_c_message_queue.hpp"

template<typename T>
class ud_c_message_producer {
public:
    ud_c_message_producer(ud_c_message_queue<T>& ref_queue) : queue_(ref_queue) {}
    
    void publish(const ud_c_message_struct<T>& message)
    {
        queue_.push(message);
    }

private:
    
    ud_c_message_queue<T>& queue_;
    std::thread thread_;
};

#endif /* ud_c_message_producer_h */
