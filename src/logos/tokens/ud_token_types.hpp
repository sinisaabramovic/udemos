#ifndef ud_token_types_hpp
#define ud_token_types_hpp

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

enum class ud_token_types {
    Identifier,
    Colon,
    Number,
    EndOfFile
};

#endif