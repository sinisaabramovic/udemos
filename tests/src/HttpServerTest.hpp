#ifndef HTTP_TEST_CLIENT_HPP
#define HTTP_TEST_CLIENT_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <curl/curl.h>

class HttpServerTest {
public:
    HttpServerTest(std::string host, uint32_t port) : m_host(host), m_port(port) {}

    std::string get(const std::string& url) {
        std::string full_url = "http://" + m_host + ":" + std::to_string(m_port) + url;
        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize CURL");
        }

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::stringstream err_stream;
            err_stream << "curl_easy_perform() failed: " << curl_easy_strerror(res);
            throw std::runtime_error(err_stream.str());
        }

        curl_easy_cleanup(curl);
        return response;
    }

private:
    std::string m_host;
    uint32_t m_port;

    static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
        std::string* response = reinterpret_cast<std::string*>(userdata);
        response->append(ptr, size * nmemb);
        return size * nmemb;
    }
};

#endif