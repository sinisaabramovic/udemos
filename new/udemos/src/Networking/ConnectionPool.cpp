//
//  ConnectionPool.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 19.03.2023..
//

#include "ConnectionPool.hpp"
#include "Connection.hpp"

void ConnectionPool::addConnection(const std::shared_ptr<Connection>& connection) {
    std::unique_lock<std::mutex> lock(mutex_);
    connection_pool_.push_back(connection);
    condition_.notify_one();
}

std::shared_ptr<Connection> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(mutex_);

    while (connection_pool_.empty()) {
        condition_.wait(lock);
    }

    auto connection = connection_pool_.front();
    connection_pool_.pop_front();
    return connection;
}
