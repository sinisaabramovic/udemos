//
//  ud_c_message_queue.hpp
//  common_tester
//
//  Created by Sinisa Abramovic on 09.03.2023..
//

#ifndef ud_c_message_queue_hpp
#define ud_c_message_queue_hpp

#include "MessageStruct.hpp"

template <typename T>
class MessageQueue
{
public:
    void push(const MessageStruct<T>& message)
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->queue_.push(message);
        this->condition_.notify_one();
    }
    
    MessageStruct<T> pop()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        
        // Wait until a message is available
        this->condition_.wait(lock, [this]() { return !this->queue_.empty(); });
        
        // Pop the oldest message
        MessageStruct<T> message = this->queue_.front();
        this->queue_.pop();
        
        return message;
    }
    
    bool empty()
    {
        return this->queue_.empty();
    }
    
private:
    std::queue<MessageStruct<T>> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
};

#endif /* ud_c_message_queue_hpp */
