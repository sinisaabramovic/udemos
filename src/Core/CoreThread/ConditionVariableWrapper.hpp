//
//  ConditionVariableWrapper.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef ConditionVariableWrapper_hpp
#define ConditionVariableWrapper_hpp

#include <condition_variable>
#include "MutexLock.hpp"

class ConditionVariableWrapper {
public:
    explicit ConditionVariableWrapper(std::condition_variable& cond_var) : cond_var_(cond_var) {}
    ~ConditionVariableWrapper() = default;

    void wait(std::unique_lock<std::mutex>& lock) { cond_var_.wait(lock); }
    void notify_one() { cond_var_.notify_one(); }
    void notify_all() { cond_var_.notify_all(); }

private:
    std::condition_variable& cond_var_;
};

#endif /* ConditionVariableWrapper_hpp */
