//
//  ThreadPool.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 18.03.2023..
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool
{
public:
    ThreadPool(uint32_t num_threads);
    ~ThreadPool();
    
    template <typename F, typename... Args>
    void enqueue(F &&f, Args &&...args);
    void stop();
    
private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_tasks_mutex;
    std::condition_variable m_tasks_cv;
    bool m_stop;
    
    void worker();
};

ThreadPool::ThreadPool(uint32_t num_threads) : m_stop(false)
{
    for (uint32_t i = 0; i < num_threads; ++i)
    {
        m_threads.emplace_back([this]()
                               { worker(); });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_tasks_mutex);
        m_stop = true;
    }
    m_tasks_cv.notify_all();
    for (auto &thread : m_threads)
    {
        thread.join();
    }
}

template <typename F, typename... Args>
void ThreadPool::enqueue(F &&f, Args &&...args)
{
    auto task = std::make_shared<std::function<void()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    {
        std::unique_lock<std::mutex> lock(m_tasks_mutex);
        // Wait until there's space in the queue
        m_tasks_cv.wait(lock, [this]()
                        { return m_stop || m_tasks.size() < 100U; });
        if (m_stop)
        {
            throw std::runtime_error("Thread pool has stopped");
        }
        m_tasks.emplace([task]()
                        { (*task)(); });
    }
    m_tasks_cv.notify_one();
}

void ThreadPool::worker()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_tasks_mutex);
            m_tasks_cv.wait(lock, [this]()
                            { return m_stop || !m_tasks.empty(); });
            if (m_stop && m_tasks.empty())
            {
                return;
            }
            task = std::move(m_tasks.front());
            m_tasks.pop();
        }
        task();
    }
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(m_tasks_mutex);
        m_stop = true;
    }
    m_tasks_cv.notify_all();
    for (auto &thread : m_threads)
    {
        thread.join();
    }
}

#endif /* ThreadPool_hpp */
