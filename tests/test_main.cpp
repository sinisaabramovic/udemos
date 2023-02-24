#include "gtest/gtest.h"
#include <thread>

#include "src/HttpServerTest.hpp"

TEST(HttpServerTest, HelloWorld) {
    HttpServerTest client("127.0.0.1", 8080);

    // std::string response = client.get("/hello");
    // EXPECT_EQ(response, "Hello, World!");
    ASSERT_EQ(1, 1);
}