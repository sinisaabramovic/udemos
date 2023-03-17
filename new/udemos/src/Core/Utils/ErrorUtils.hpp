//
//  ErrorUtils.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef ErrorUtils_hpp
#define ErrorUtils_hpp

#include <string>
#include <system_error>

namespace ErrorUtils {

std::string formatError(const std::system_error& error);

}  // namespace ErrorUtils

#endif /* ErrorUtils_hpp */
