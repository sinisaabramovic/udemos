#include "udConnection.hpp"
#include "udHttpRequest.hpp"
#include "udRoute.hpp"
#include "udRouter.hpp"

udRouter::udRouter() {}

void udRouter::AddRoute(std::shared_ptr<udRoute> route)
{
    routes_.push_back(route);
}

void udRouter::HandleRequest(std::shared_ptr<udHttpRequest> request, std::shared_ptr<udConnection> connection)
{
    request_stream_->write(request->GetBody());

    std::string response_data;
    auto response_observer = std::make_shared<udObserver>();
    response_observer->update = [&](const std::string &message)
    {
        response_data = message;
    };
    connection->response_stream_->attach(response_observer);

    for (auto &route : routes_)
    {
        if (route->Matches(request))
        {
            auto response = route->Handle(request);
            connection->Send(response);
            break;
        }
    }

    connection->response_stream_->detach(response_observer);
    connection->response_stream_.reset();

    auto response = std::make_shared<udHttpResponse>();
    response->SetBody(response_data);
    connection->Send(response);
}

void udRouter::attach_request_stream(std::shared_ptr<udRequestStream> request_stream)
{
    request_stream_ = request_stream;
}