#ifndef UD_FLUX_HPP
#define UD_FLUX_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <sstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class udObserver
{
public:
    virtual ~udObserver() = default;
    virtual void update(const std::string &message) = 0;
};

class udSubject : public std::enable_shared_from_this<udSubject>
{
public:
    virtual ~udSubject() = default;

    void attach(std::shared_ptr<udObserver> observer)
    {
        observers_.push_back(observer);
    }

    void detach(std::shared_ptr<udObserver> observer)
    {
        auto it = std::find(observers_.begin(), observers_.end(), observer);
        if (it != observers_.end())
        {
            observers_.erase(it);
        }
    }

    void notify(const std::string &message)
    {
        for (auto observer : observers_)
        {
            observer->update(message);
        }
    }

    std::shared_ptr<udSubject> shared_from_this()
    {
        return std::dynamic_pointer_cast<udSubject>(std::enable_shared_from_this<udSubject>::shared_from_this());
    }

private:
    std::vector<std::shared_ptr<udObserver>> observers_;
};

class udRequestStream : public udSubject
{
public:
    void write(const std::string &data)
    {
        notify(data);
    }
};

class udResponseStream : public udSubject
{
public:
    void send(const std::string &data)
    {
        notify(data);
    }
};

#endif