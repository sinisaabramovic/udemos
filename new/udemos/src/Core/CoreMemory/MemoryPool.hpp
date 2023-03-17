//
//  MemoryPool.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef MemoryPool_hpp
#define MemoryPool_hpp

#include <cstdlib>
#include <vector>
#include <memory>
#include <mutex>

#include "MemoryBlock.hpp"

class MemoryPool {
public:
    MemoryPool(size_t blockSize, size_t initialPoolSize);
    ~MemoryPool();

    void* allocate(size_t size);
    void deallocate(void* ptr);

private:
    size_t blockSize_;
    size_t initialPoolSize_;
    std::vector<std::unique_ptr<char[]>> pool_;
    std::vector<MemoryBlock> blocks_;
    MemoryBlock* freeList_;
    std::mutex mutex_;
};

#endif /* MemoryPool_hpp */
