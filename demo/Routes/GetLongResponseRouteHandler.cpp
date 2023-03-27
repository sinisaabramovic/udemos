//
//  GetLongResponseRouteHandler.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 24.03.2023..
//

#include <chrono>
#include <thread>
#include "../../src/Protocol/HttpModels/HttpResponse.hpp"
#include "../../src/Protocol/HttpModels/HttpRequest.hpp"
#include "GetLongResponseRouteHandler.hpp"

// Simulates long response

GetLongResponseRouteHandler::GetLongResponseRouteHandler(const std::string& method, const std::string& path) : RouteHandler(method, path) {}

std::shared_ptr<HttpResponse> GetLongResponseRouteHandler::handleRequest(const HttpRequest& request) {
    auto response = std::make_shared<HttpResponse>();
    response->setStatusCode(HttpResponseStatus::OK);
    response->addHeader("Content-Type", "application/json");
    
    int n = 12;
    
    long long prev = 0, curr = 1;
    for (int i = 0; i < n; i++) {
        long long next = prev + curr;
        prev = curr;
        curr = next;
    }
    std::string fibResult = "Fibonacci(" + std::to_string(n) + ") = " + std::to_string(curr);
    
    long long factorial = 1;
    for (int i = 1; i <= n; i++) {
        factorial *= i;
    }
    std::string factResult = "Factorial(" + std::to_string(n) + ") = " + std::to_string(factorial);
    
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    nlohmann::json result;
    result["fibonacci"] = fibResult;
    result["factorial"] = factResult;
    
    response->setBody(result.dump());
    
    return response;
}
