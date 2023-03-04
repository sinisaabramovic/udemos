#ifndef UD_THREADPOOL_HPP
#define UD_THREADPOOL_HPP

#include <queue>
#include <functional>

class udThreadPool
{
public:
    udThreadPool(size_t num_threads)
    {
        for (size_t i = 0; i < num_threads; ++i)
        {
            threads_.emplace_back([this]
                                  {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        condition_.wait(lock, [this] { return !tasks_.empty() || stopped_; });
                        if (stopped_ && tasks_.empty()) {
                            break;
                        }
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                } });
        }
    }

    ~udThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stopped_ = true;
        }
        condition_.notify_all();
        for (auto &thread : threads_)
        {
            thread.join();
        }
    }

    template <typename F>
    void submit(F &&f)
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace(std::forward<F>(f));
        }
        condition_.notify_one();
    }

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stopped_ = false;
};

#endif