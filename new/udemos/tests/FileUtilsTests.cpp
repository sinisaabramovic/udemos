//
//  FileUtilsTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <gtest/gtest.h>
#include <cstdio>
#include "FileUtils.hpp"

TEST(FileUtilsTests, ReadWriteFile) {
    std::string path = "test.txt";
    std::string content = "Hello, World!";

    // Write content to file
    FileUtils::writeFile(path, content);

    // Read content from file
    std::string readContent = FileUtils::readFile(path);

    // Check if the read content is the same as the written content
    EXPECT_EQ(content, readContent);

    // Remove the test file
    std::remove(path.c_str());
}
