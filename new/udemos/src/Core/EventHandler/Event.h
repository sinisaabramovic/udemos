//
//  Event.h
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef Event_h
#define Event_h

#include <memory>

class Event {
public:
    enum class EventType { NewConnection, Connection, DataReceived, DataSent, Error, Custom };
    
    explicit Event(int fd, EventType type) : fd_(fd), type_(type) {}
    virtual ~Event() = default;
    
    EventType getType() const { return type_; }
    
    int getFd() const { return fd_; }
    
private:
    int fd_;
    EventType type_;
};
using EventPtr = std::shared_ptr<Event>;


#endif /* Event_h */
