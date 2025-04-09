//
// Created by Administrator on 25-4-9.
//

#ifndef EVENTPOOL_H
#define EVENTPOOL_H

#include <Arduino.h>
#include "Event.h"

// 事件池的大小，假设最多支持3个事件
#define EVENT_POOL_SIZE 3


class EventPool {
public:
    EventPool();
    ~EventPool();
    Event* getEvent(EventType type);
    void recycleEvent(Event* event);
    void reset();

private:
    Event eventPool[EVENT_POOL_SIZE];  // 事件池
    int currentEventIndex;  // 当前池中的事件索引
};



#endif //EVENTPOOL_H
