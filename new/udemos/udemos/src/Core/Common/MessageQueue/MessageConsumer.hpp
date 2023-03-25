//
//  ud_c_message_consumer.hpp
//  common_tester
//
//  Created by Sinisa Abramovic on 09.03.2023..
//

#ifndef ud_c_message_consumer_hpp
#define ud_c_message_consumer_hpp

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <queue>

#include "MessageStruct.hpp"
#include "MessageQueue.hpp"

template <typename T>
class MessageConsumer
{
public:
    MessageConsumer(MessageQueue<T>& ref_queue) : queue_(ref_queue), pauseFlag_(false), stopFlag_(false), queue_empty_(true) {}
    
    void start(std::function<bool(const MessageStruct<T>&)> process_message_callback)
    {
        this->thread_ = std::thread(&MessageConsumer::run, this, process_message_callback);
    }
    
    void pause()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->pauseFlag_ = true;
        this->condition_.notify_one();
    }
    
    void resume()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->pauseFlag_ = false;
        this->condition_.notify_one();
    }
    
    void stop()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->stopFlag_ = true;
        this->condition_.notify_one();
    }
    
    void join() {
        if (this->thread_.joinable())
        {
            this->thread_.join();
        }
    }
    
    void push(const MessageStruct<T>& message)
    {
        {
            std::unique_lock<std::mutex> lock(this->mutex_);
            this->queue_.push(message);
        }
        this->condition_.notify_one();
    }
    
private:
    void run(std::function<bool(const MessageStruct<T>&)> process_message_callback)
    {
        while (!this->stopFlag_)
        {
            MessageStruct<T> message;
            bool message_received = false;
            {
                std::unique_lock<std::mutex> lock(this->mutex_);
                this->condition_.wait(lock, [this]() { return !this->queue_.empty(); });
                if (!this->queue_.empty())
                {
                    message = this->queue_.pop();
                    message_received = true;
                }
            }
            
            if (message_received && !process_message_callback(message))
            {
                break;
            }
            
            {
                std::unique_lock<std::mutex> lock(this->mutex_);
                while (this->pauseFlag_)
                {
                    this->condition_.wait(lock);
                }
            }
        }
    }
    
    MessageQueue<T>& queue_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool pauseFlag_;
    bool stopFlag_;
    bool queue_empty_;
};


#endif /* ud_c_message_consumer_h */
