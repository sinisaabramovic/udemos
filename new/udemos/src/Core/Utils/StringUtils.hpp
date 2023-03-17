//
//  StringUtils.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef StringUtils_hpp
#define StringUtils_hpp

#include <string>
#include <algorithm>

namespace StringUtils {

std::string toLower(const std::string& input);
std::string toUpper(const std::string& input);
std::string trim(const std::string& input);

}  // namespace StringUtils

#endif /* StringUtils_hpp */
