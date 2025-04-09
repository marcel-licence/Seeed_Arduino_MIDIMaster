#include "Event.h"

Event::Event(EventType type)
    : _type(type)
{

}

Event::Event(EventType type, unsigned long timestamp)
    : _type(type)
    , _timestamp(timestamp)
{

}

Event::~Event()
{

}

EventType Event::getType() const
{
    return _type;
}

void Event::setType(EventType type)
{
    _type = type;
}

bool Event::isInUse() const
{
    return _inUse;
}

void Event::setInUse(bool inUse)
{
    _inUse = inUse;
}

unsigned long Event::getTimestamp() const
{
    return _timestamp;
}
