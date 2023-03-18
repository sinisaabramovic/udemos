//
//  ThreadPool.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();
    
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
    
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable tasks_cv_;
    std::atomic<bool> stop_;
};

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mutex_);
                    tasks_cv_.wait(lock, [this]() { return !tasks_.empty() || stop_; });
                    
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    stop_ = true;
    tasks_cv_.notify_all();
    
    for (auto& worker : workers_) {
        worker.join();
    }
}

template<typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    
    auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    
    std::future<return_type> result = task->get_future();
    {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        if (stop_) {
            throw std::runtime_error("ThreadPool is stopped");
        }
        tasks_.emplace([task]() { (*task)(); });
    }
    tasks_cv_.notify_one();
    return result;
}

#endif /* ThreadPool_hpp */
