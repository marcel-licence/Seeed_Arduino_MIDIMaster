//
// Created by Administrator on 25-4-9.
//

#include "EventPool.h"

EventPool::EventPool()
    :currentEventIndex(0)
{
    for (int i = 0; i < EVENT_POOL_SIZE; i++) {
        eventPool[i] = Event();  // default event : None
    }
    reset();
}

EventPool::~EventPool()
{

}

Event* EventPool::getEvent(EventType type)
{
    for (int i = 0; i < EVENT_POOL_SIZE; i++) {
        if (!eventPool[i].isInUse()) {
            eventPool[i].setType(type);
            eventPool[i].setInUse(true);
            return &eventPool[i];
        }
    }
    return nullptr;
}

void EventPool::recycleEvent(Event* event)
{
    if (event) {
        event->setInUse(false);
    }
}

void EventPool::reset()
{
    //reset pool
    for (int i = 0; i < EVENT_POOL_SIZE; i++) {
        eventPool[i].setInUse(false);
    }
    currentEventIndex = 0;
}


