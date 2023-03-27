//
//  BaseService.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef BaseService_hpp
#define BaseService_hpp

#include <unordered_map>
#include <memory>
#include "../Protocol/ProtocolHandler.h"
#include "../Core/Config/Configuration.hpp"
#include "../Networking/Connection.hpp"

class HttpProtocolHandler;
class ConnectionPool;

class HttpService {
public:
    HttpService(const Configuration& config);
    ~HttpService();
    void run();
    void stop();
    void registerProtocolHandler(const std::string& protocol, std::unique_ptr<ProtocolHandler> handler);
    
private:
    void prepareServerSocket();
    void processNewConnection(ConnectionPool &connection_pool, ProtocolHandler &http_handler, std::vector<std::future<void>> &connection_futures);
    void cleanUpConnectionFutures(std::vector<std::future<void>> &connection_futures);

    
private:
    bool stop_flag_;
    Configuration& config_;
    std::shared_ptr<Socket> server_socket_;
    std::unordered_map<std::string, std::unique_ptr<ProtocolHandler>> protocol_handlers_;
};

#endif /* BaseService_hpp */
