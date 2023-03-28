//
//  AddressResolver.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "AddressResolver.hpp"
#include <string.h>
#include <cstring>
#include <stdexcept>

std::vector<std::string> AddressResolver::resolve(const std::string& hostname) {
    std::vector<std::string> addresses;
    struct addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname.c_str(), nullptr, &hints, &res);
    if (status != 0) {
        throw std::runtime_error(gai_strerror(status));
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        void* addr;
        char ipstr[INET6_ADDRSTRLEN];

        if (p->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        addresses.push_back(ipstr);
    }

    freeaddrinfo(res);
    return addresses;
}
