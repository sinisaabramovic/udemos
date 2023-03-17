//
//  NetworkingTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <gtest/gtest.h>
#include "Socket.hpp"
#include "Connection.hpp"
#include "AddressResolver.hpp"
#include "EventLoop.h"

TEST(SocketTests, BindAndListen) {
    EventLoop loop;
    Socket socket(loop);
    socket.bind("127.0.0.1", 8080); // Bind to an available port
    socket.listen();
}

TEST(SocketTests, Connect) {
    EventLoop loop;
    
    try {
        Socket socket(loop);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        inet_pton(AF_INET, "93.184.216.34", &addr.sin_addr);
        
        int result = socket.connect(reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
        ASSERT_TRUE(result == 0 || (result == -1 && errno == EINPROGRESS));
        
        // Wait for the socket to become writable (connection established)
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(socket.fd(), &wfds);
        timeval timeout = {5, 0}; // 5 seconds timeout
        int sel_result = select(socket.fd() + 1, nullptr, &wfds, nullptr, &timeout);
        
        ASSERT_GT(sel_result, 0) << "select() timed out";
        ASSERT_TRUE(FD_ISSET(socket.fd(), &wfds)) << "Socket is not writable";
    } catch (const std::exception& e) {
        FAIL() << e.what();
    }
}


TEST(AddressResolverTests, ResolveIPv4) {
    auto addresses = AddressResolver::resolve("www.example.com");
    EXPECT_FALSE(addresses.empty());
    EXPECT_NE(std::find(addresses.begin(), addresses.end(), "93.184.216.34"), addresses.end());
}

TEST(AddressResolverTests, ResolveIPv6) {
    try {
        auto addresses = AddressResolver::resolve("ipv6.test-ipv6.com");
        EXPECT_FALSE(addresses.empty());
        
        bool ipv6_found = false;
        for (const auto& address : addresses) {
            if (address.find(':') != std::string::npos) {
                ipv6_found = true;
                break;
            }
        }
        EXPECT_TRUE(ipv6_found);
    } catch (const std::runtime_error& e) {
        std::cerr << "Warning: Unable to resolve IPv6 address: " << e.what() << std::endl;
    }
}

