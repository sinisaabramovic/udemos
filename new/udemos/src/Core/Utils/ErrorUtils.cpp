//
//  ErrorUtils.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "ErrorUtils.hpp"

namespace ErrorUtils {

std::string formatError(const std::system_error& error) {
    return "Error: " + std::to_string(error.code().value()) + " - " + error.what();
}

}  // namespace ErrorUtils
