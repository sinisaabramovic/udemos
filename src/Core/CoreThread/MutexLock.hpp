//
//  MutexLock.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef MutexLock_hpp
#define MutexLock_hpp

#include <mutex>

class MutexLock {
public:
    explicit MutexLock(std::mutex& mutex) : mutex_(mutex) { mutex_.lock(); }
    ~MutexLock() { mutex_.unlock(); }
    
    MutexLock(const MutexLock&) = delete;
    MutexLock& operator=(const MutexLock&) = delete;
    
private:
    std::mutex& mutex_;
};

#endif /* MutexLock_hpp */
