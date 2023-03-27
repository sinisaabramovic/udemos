//
//  MemoryPool.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "MemoryPool.hpp"

// MemoryPool.cpp
MemoryPool::MemoryPool(size_t blockSize, size_t initialPoolSize)
    : blockSize_(blockSize), initialPoolSize_(initialPoolSize), freeList_(nullptr) {
}

MemoryPool::~MemoryPool() = default;

void* MemoryPool::allocate(size_t size) {
    if (size > blockSize_) {
        throw std::runtime_error("Requested size exceeds block size");
    }
    
    std::unique_lock<std::mutex> lock(mutex_);
    if (!freeList_) {
        size_t newPoolSize = blocks_.empty() ? initialPoolSize_ : 2 * blocks_.size();
        pool_.emplace_back(new char[newPoolSize * blockSize_]);
        blocks_.reserve(blocks_.size() + newPoolSize);
        
        for (size_t i = 0; i < newPoolSize; ++i) {
            blocks_.emplace_back(pool_.back().get() + i * blockSize_);
            blocks_.back().next = freeList_;
            freeList_ = &blocks_.back();
        }
    }
    
    MemoryBlock* result = freeList_;
    freeList_ = freeList_->next;
    return result->data;
}

void MemoryPool::deallocate(void* ptr) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    for (size_t blockIdx = 0; blockIdx < blocks_.size(); ++blockIdx) {
        if (blocks_[blockIdx].data == ptr) {
            blocks_[blockIdx].next = freeList_;
            freeList_ = &blocks_[blockIdx];
            return;
        }
    }
    
    throw std::runtime_error("Invalid pointer");
}


