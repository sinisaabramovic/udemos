#include "reactive/udFlux.hpp"
#include "udHttpRequest.hpp"
#include "udHttpResponse.hpp"
#include "udRoute.hpp"

udRoute::udRoute(const std::string &method, const std::string &path, std::shared_ptr<udRequestStream> request_stream)
    : method_(method), path_(path), path_regex_(path), request_stream_(request_stream)
{
    request_stream_->attach(shared_from_this());
}

bool udRoute::Matches(std::shared_ptr<udHttpRequest> request)
{
    return request->GetMethod() == method_ && std::regex_match(request->GetPath(), path_regex_);
}

void udRoute::update(const std::string &message) override
{
    auto request = std::make_shared<udHttpRequest>(message);
    auto response = std::make_shared<udHttpResponse>();
    Handle(request, response);
    response_stream_->send(response->ToString());
}

void udRoute::set_response_stream(std::shared_ptr<udResponseStream> response_stream)
{
    response_stream_ = response_stream;
}

std::string udRoute::GetMethod() const
{
    return method_;
}

std::string udRoute::GetPath() const
{
    return path_;
}