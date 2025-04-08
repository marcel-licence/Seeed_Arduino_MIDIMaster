#ifndef EVENT_HPP
#define EVENT_HPP


enum EventType
{
    None,
    APressed,
    BPressed,
    CPressed,
    DPressed,
    AReleased,
    BReleased,
    CReleased,
    DReleased,
    BtnReleased,
    ALongPressed,
    BLongPressed,
    CLongPressed,
    DLongPressed,
};



class Event
{
public:
    Event(EventType type)
        : _type(type)
    {

    }

    Event(EventType type, unsigned long timestamp)
        : _type(type)
        , _timestamp(timestamp)
    {

    }

    virtual ~Event()
    {

    }

    EventType getType() const
    {
        return _type;
    }

    unsigned long getTimestamp() const
    {
        return _timestamp;
    }

private:
    EventType _type;
    unsigned long _timestamp;
};

#endif // EVENT_HPP