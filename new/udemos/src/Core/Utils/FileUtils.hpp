//
//  FileUtils.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef FileUtils_hpp
#define FileUtils_hpp

#include <string>
#include <vector>

namespace FileUtils {

std::string readFile(const std::string& path);
void writeFile(const std::string& path, const std::string& content);

}  // namespace FileUtils


#endif /* FileUtils_hpp */
