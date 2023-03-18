//
//  EventHandlingTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <stdio.h>
#include <iostream>
#include <gtest/gtest.h>
#include "Event.h"
#include "EventQueue.h"
#include "EventLoop.h"

//TEST(EventQueueTest, PushPopAndEmpty) {
//    EventQueue eventQueue;
//    EXPECT_TRUE(eventQueue.empty());
//
//    auto event = std::make_shared<Event>(Event::EventType::Connection);
//    eventQueue.push(event);
//
//    EXPECT_FALSE(eventQueue.empty());
//    EventPtr poppedEvent = eventQueue.pop();
//
//    EXPECT_TRUE(eventQueue.empty());
//    ASSERT_NE(poppedEvent, nullptr);
//    EXPECT_EQ(poppedEvent->getType(), Event::EventType::Connection);
//}
//
//void processEventCallback(EventPtr event, int& counter) {
//    if (event->getType() == Event::EventType::Connection) {
//        ++counter;
//    }
//}
//
//TEST(EventLoopTest, AddAndProcessEvent) {
//    EventLoop eventLoop;
//    eventLoop.run();
//
//    int eventCounter = 0;
//    eventLoop.setProcessEventCallback(std::bind(processEventCallback, std::placeholders::_1, std::ref(eventCounter)));
//
//    auto event1 = std::make_shared<Event>(Event::EventType::Connection);
//    auto event2 = std::make_shared<Event>(Event::EventType::Connection);
//    auto event3 = std::make_shared<Event>(Event::EventType::DataReceived);
//
//    eventLoop.addEvent(event1);
//    eventLoop.addEvent(event2);
//    eventLoop.addEvent(event3);
//
//    // Give the event loop some time to process events.
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//
//    EXPECT_EQ(eventCounter, 2);
//    eventLoop.stop();
//}

