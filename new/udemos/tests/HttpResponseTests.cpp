////
////  HttpResponseTests.cpp
////  udemos
////
////  Created by Sinisa Abramovic on 17.03.2023..
////
//
//#include <gtest/gtest.h>
//#include "HttpResponse.hpp"
//
//class HttpResponseTests : public ::testing::Test {
//protected:
//    HttpResponseTests() {}
//
//    void SetUp() override {}
//    void TearDown() override {}
//};
//
//TEST_F(HttpResponseTests, DefaultConstructor) {
//    HttpResponse response;
//
//    EXPECT_EQ(response.getStatusCode(), HttpResponseStatus::OK);
//    EXPECT_EQ(response.getContentType(), "text/plain");
//    EXPECT_EQ(response.getBody(), "");
//}
//
//TEST_F(HttpResponseTests, CustomConstructor) {
//    HttpResponse response(HttpResponseStatus::NOT_FOUND, "application/json", "{\"error\":\"Not Found\"}");
//
//    EXPECT_EQ(response.getStatusCode(), HttpResponseStatus::NOT_FOUND);
//    EXPECT_EQ(response.getContentType(), "application/json");
//    EXPECT_EQ(response.getBody(), "{\"error\":\"Not Found\"}");
//}
//
//TEST_F(HttpResponseTests, SettersAndGetters) {
//    HttpResponse response;
//
//    response.setStatusCode(HttpResponseStatus::BAD_REQUEST);
//    response.setContentType("application/json");
//    response.setBody("{\"error\":\"Bad Request\"}");
//
//    EXPECT_EQ(response.getStatusCode(), HttpResponseStatus::BAD_REQUEST);
//    EXPECT_EQ(response.getContentType(), "application/json");
//    EXPECT_EQ(response.getBody(), "{\"error\":\"Bad Request\"}");
//}
//
//TEST_F(HttpResponseTests, HeadersManipulation) {
//    HttpResponse response;
//    std::unordered_map<std::string, std::string> headers = {
//        {"Content-Encoding", "gzip"},
//        {"X-Custom-Header", "CustomValue"}
//    };
//
//    response.setHeaders(headers);
//    response.addHeader("X-Another-Header", "AnotherValue");
//
//    auto responseHeaders = response.getHeaders();
//
//    EXPECT_EQ(responseHeaders["Content-Encoding"], "gzip");
//    EXPECT_EQ(responseHeaders["X-Custom-Header"], "CustomValue");
//    EXPECT_EQ(responseHeaders["X-Another-Header"], "AnotherValue");
//}
//
//TEST_F(HttpResponseTests, ToString) {
//    HttpResponse response(HttpResponseStatus::NOT_FOUND, "application/json", "{\"error\":\"Not Found\"}");
//    std::string expectedOutput = "HTTP/1.1 404 Not Found\r\nContent-Length: 21\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type, Authorization\r\n\r\n{\"error\":\"Not Found\"}";
//
//    EXPECT_EQ(response.toString(), expectedOutput);
//}
