//
// Created by Administrator on 25-4-9.
//

#ifndef EVENTPOOL_H
#define EVENTPOOL_H

#include <Arduino.h>
#include "Event.h"

// size of event pool now is 3
#define EVENT_POOL_SIZE 3


class EventPool {
public:
    EventPool();
    ~EventPool();
    Event* getEvent(EventType type);
    void recycleEvent(Event* event);
    void reset();

private:
    Event eventPool[EVENT_POOL_SIZE];  
    int currentEventIndex;  // index
};



#endif //EVENTPOOL_H
