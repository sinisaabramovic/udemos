//
//  ConnectionPool.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 19.03.2023..
//

#ifndef ConnectionPool_hpp
#define ConnectionPool_hpp

#include <deque>
#include <mutex>
#include <condition_variable>

class Connection;

class ConnectionPool {
public:
    void addConnection(const std::shared_ptr<Connection>& connection);
    std::shared_ptr<Connection> getConnection();

private:
    std::deque<std::shared_ptr<Connection>> connection_pool_;
    std::mutex mutex_;
    std::condition_variable condition_;
};


#endif /* ConnectionPool_hpp */
