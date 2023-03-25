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

#include "MessageStruct.hpp"
#include "MessageQueue.hpp"

template<typename T>
class MessageProducer {
public:
    MessageProducer(MessageQueue<T>& ref_queue) : queue_(ref_queue) {}
    
    void publish(const MessageStruct<T>& message)
    {
        queue_.push(message);
    }
    
private:
    
    MessageQueue<T>& queue_;
    std::thread thread_;
};

#endif /* ud_c_message_producer_h */
