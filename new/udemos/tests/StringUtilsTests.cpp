//
//  StringUtilsTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <gtest/gtest.h>
#include "StringUtils.hpp"

TEST(StringUtilsTests, ToLower) {
    std::string input = "HeLLo WoRLd!";
    std::string expected = "hello world!";
    EXPECT_EQ(StringUtils::toLower(input), expected);
}

TEST(StringUtilsTests, ToUpper) {
    std::string input = "HeLLo WoRLd!";
    std::string expected = "HELLO WORLD!";
    EXPECT_EQ(StringUtils::toUpper(input), expected);
}

TEST(StringUtilsTests, Trim) {
    std::string input = "   HeLLo WoRLd!  \t\n\r";
    std::string expected = "HeLLo WoRLd!";
    EXPECT_EQ(StringUtils::trim(input), expected);
}
