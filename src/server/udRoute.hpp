#ifndef UD_ROUTE_H
#define UD_ROUTE_H

#include <iostream>
#include <memory>
#include <regex>

class udObserver;
class udRequestStream;
class udResponseStream;
class udHttpResponse;
class udHttpRequest;

class udRoute : public udObserver, public std::enable_shared_from_this<udRoute>
{
private:
    std::string method_;
    std::string path_;
    std::regex path_regex_;
    std::shared_ptr<udRequestStream> request_stream_;
    std::shared_ptr<udResponseStream> response_stream_;

public:
    udRoute(const std::string &method, const std::string &path, std::shared_ptr<udRequestStream> request_stream);
    bool Matches(std::shared_ptr<udHttpRequest> request);
    virtual void Handle(std::shared_ptr<udHttpRequest> request, std::shared_ptr<udHttpResponse> response) = 0;
    void update(const std::string &message) override;
    void set_response_stream(std::shared_ptr<udResponseStream> response_stream);
    std::string GetMethod() const;
    std::string GetPath() const;
};

#endif // UD_ROUTE_H
