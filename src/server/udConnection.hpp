#ifndef UD_CONNECTION_H
#define UD_CONNECTION_H

#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

class udRouter;
class udObserver;
class udRequestStream;
class udResponseStream;

class udConnection : public std::enable_shared_from_this<udConnection>, public udObserver
{
private:
    int socket_;
    std::shared_ptr<udRouter> router_;
    std::shared_ptr<udRequestStream> request_stream_;
    std::shared_ptr<udResponseStream> response_stream_;

public:
    udConnection(int socket, std::shared_ptr<udRouter> router);
    void Start();
    void Send(std::shared_ptr<udHttpResponse> response);
    void Close();
    void update(const std::string &message) override;
    void attach_request_stream(std::shared_ptr<udRequestStream> request_stream);
    void attach_response_stream(std::shared_ptr<udResponseStream> response_stream);
};

#endif // UD_CONNECTION_H
