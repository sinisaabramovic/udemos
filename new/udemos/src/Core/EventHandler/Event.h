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
    enum class EventType { Connection, DataReceived, DataSent, Error, Custom };
    
    explicit Event(EventType type) : type_(type) {}
    virtual ~Event() = default;
    
    EventType getType() const { return type_; }
    
private:
    EventType type_;
};
using EventPtr = std::shared_ptr<Event>;

#endif /* Event_h */
