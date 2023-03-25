//
//  CodableAndDecodable.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 19.03.2023..
//

#ifndef CodableAndDecodable_hpp
#define CodableAndDecodable_hpp

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Codable {
public:
    virtual json toJson() const = 0;
};

class Decodable {
public:
    virtual void fromJson(const json& j) = 0;
};

#endif /* CodableAndDecodable_hpp */
