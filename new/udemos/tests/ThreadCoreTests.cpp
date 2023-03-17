//
//  ThreadCoreTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <stdio.h>
#include <iostream>
#include <gtest/gtest.h>
#include "ThreadWrapper.hpp"
#include "MutexLock.hpp"
#include "ConditionVariableWrapper.hpp"
#include "ThreadSafeQueue.hpp"

TEST(ThreadWrapperTest, ThreadExecution) {
    std::atomic<int> counter(0);
    {
        ThreadWrapper thread([&]() { counter += 1; });
    }
    ASSERT_EQ(counter.load(), 1);
}

TEST(MutexLockTest, MutexSynchronization) {
    std::mutex mtx;
    int value = 0;

    auto increment = [&]() {
        for (int i = 0; i < 10000; ++i) {
            MutexLock lock(mtx);
            ++value;
        }
    };

    {
        ThreadWrapper thread1(increment);
        ThreadWrapper thread2(increment);
    }

    ASSERT_EQ(value, 20000);
}

TEST(ConditionVariableWrapperTest, WaitAndNotify) {
    std::mutex mtx;
    std::condition_variable cv;
    ConditionVariableWrapper cv_wrapper(cv);
    bool ready = false;
    
    ThreadWrapper thread([&]() {
        std::unique_lock<std::mutex> lock(mtx);
        while (!ready) {
            cv_wrapper.wait(lock);
        }
        ASSERT_TRUE(ready);
    });
    
    {
        std::unique_lock<std::mutex> lock(mtx);
        ready = true;
    }
    cv_wrapper.notify_one();
}

TEST(ThreadSafeQueueTest, PushPopAndEmpty) {
    ThreadSafeQueue<int> queue;
    ASSERT_TRUE(queue.empty());
    
    queue.push(1);
    ASSERT_FALSE(queue.empty());
    
    int value;
    ASSERT_TRUE(queue.try_pop(value));
    ASSERT_EQ(value, 1);
    ASSERT_TRUE(queue.empty());
}


