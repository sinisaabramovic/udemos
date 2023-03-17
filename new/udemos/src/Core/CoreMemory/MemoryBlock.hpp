//
//  MemoryBlock.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef MemoryBlock_hpp
#define MemoryBlock_hpp

class MemoryBlock {
public:
    MemoryBlock(char* data) : data(data), next(nullptr) {}

    char* data;
    MemoryBlock* next;
};


#endif /* MemoryBlock_hpp */
