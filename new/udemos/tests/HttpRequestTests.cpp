////
////  UdHttpRequestTests.cpp
////  udemos
////
////  Created by Sinisa Abramovic on 17.03.2023..
////
//
//#include <gtest/gtest.h>
//#include "HttpRequest.hpp"
//
//TEST(HttpRequestTests, ParseBasicRequest) {
//    std::string request = "GET /test?param1=value1&param2=value2 HTTP/1.1\r\n"
//                          "Host: example.com\r\n"
//                          "Connection: keep-alive\r\n"
//                          "\r\n";
//    HttpRequest httpRequest(request);
//
//    EXPECT_EQ("GET", httpRequest.getMethod());
//    EXPECT_EQ("/test", httpRequest.getPath());
//}
//
//TEST(HttpRequestTests, ParseRequestWithBody) {
//    std::string request = "POST /test HTTP/1.1\r\n"
//                          "Host: example.com\r\n"
//                          "Content-Type: application/json\r\n"
//                          "Content-Length: 27\r\n"
//                          "\r\n"
//                          "{\"key1\":\"value1\",\"key2\":42}";
//    HttpRequest httpRequest(request);
//
//    EXPECT_EQ("POST", httpRequest.getMethod());
//    EXPECT_EQ("/test", httpRequest.getPath());
//    EXPECT_EQ("{\"key1\":\"value1\",\"key2\":42}", httpRequest.getBody());
//}
//
//TEST(HttpRequestTests, IsValidJson) {
//    std::string validJson = "{\"key1\":\"value1\",\"key2\":42}";
//    std::string invalidJson = "{key1:\"value1\",key2:42}";
//
//    HttpRequest httpRequest("GET /test HTTP/1.1\r\nHost: example.com\r\n\r\n");
//
//    EXPECT_TRUE(httpRequest.isValidJson(validJson));
//    EXPECT_FALSE(httpRequest.isValidJson(invalidJson));
//}
//
//TEST(HttpRequestTests, CookieParsing) {
//    std::string request =
//        "GET / HTTP/1.1\r\n"
//        "Host: example.com\r\n"
//        "Cookie: key1=value1; key2=value2; key3=value3\r\n"
//        "\r\n";
//
//    HttpRequest httpRequest(request);
//
//    std::unordered_map<std::string, std::string> expectedCookies = {
//        {"key1", "value1"},
//        {"key2", "value2"},
//        {"key3", "value3"}
//    };
//
//    ASSERT_EQ(httpRequest.getCookies(), expectedCookies);
//}
//
//TEST(HttpRequestTests, MultipartFormDataParsing) {
//    std::string request =
//        "POST / HTTP/1.1\r\n"
//        "Content-Type: multipart/form-data; boundary=exampleboundary\r\n"
//        "Content-Length: 353\r\n"
//        "\r\n"
//        "--exampleboundary\r\n"
//        "Content-Disposition: form-data; name=\"field1\"\r\n"
//        "\r\n"
//        "value1\r\n"
//        "--exampleboundary\r\n"
//        "Content-Disposition: form-data; name=\"field2\"\r\n"
//        "\r\n"
//        "value2\r\n"
//        "--exampleboundary\r\n"
//        "Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\n"
//        "Content-Type: text/plain\r\n"
//        "\r\n"
//        "file_content_here\r\n"
//        "--exampleboundary--\r\n";
//
//    HttpRequest httpRequest(request);
//
//    std::unordered_map<std::string, std::string> expectedFormFields = {
//        {"field1", "value1"},
//        {"field2", "value2"}
//    };
//
//    std::unordered_map<std::string, std::string> expectedFileUploads = {
//        {"test.txt", "file_content_here"}
//    };
//
//    ASSERT_EQ(httpRequest.getFormFields(), expectedFormFields);
//    ASSERT_EQ(httpRequest.getFileUploads(), expectedFileUploads);
//}
//
//
