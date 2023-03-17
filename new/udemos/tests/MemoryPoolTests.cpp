//
//  MemoryPoolTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <iostream>
#include <gtest/gtest.h>
#include "MemoryPool.hpp"

TEST(MemoryPoolTest, AllocationAndDeallocation) {
    MemoryPool pool(64, 4);
    
    std::vector<void*> allocated_blocks;
    
    // Allocate 4 blocks of memory
    for (int i = 0; i < 4; ++i) {
        void* block = pool.allocate(64);
        ASSERT_NE(block, nullptr);
        allocated_blocks.push_back(block);
    }
    
    // Deallocate the memory blocks
    for (auto block : allocated_blocks) {
        ASSERT_NO_THROW(pool.deallocate(block));
    }
}

TEST(MemoryPoolTest, PoolResizing) {
    MemoryPool pool(64, 1);
    
    // Allocate 2 blocks of memory, forcing the pool to resize
    void* block1 = pool.allocate(64);
    void* block2 = pool.allocate(64);
    
    ASSERT_NE(block1, nullptr);
    ASSERT_NE(block2, nullptr);
    ASSERT_NO_THROW(pool.deallocate(block1));
    ASSERT_NO_THROW(pool.deallocate(block2));
}

TEST(MemoryPoolTest, InvalidPointerDeallocation) {
    MemoryPool pool(64, 4);
    void* invalid_ptr = nullptr;
    
    ASSERT_THROW(pool.deallocate(invalid_ptr), std::runtime_error);
}

