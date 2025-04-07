#ifndef EVENT_HPP
#define EVENT_HPP


enum EventType
{
    NoEvent,
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


class ButtonEvent: public Event {
public:
    ButtonEvent(EventType type, int buttonID)
        : Event(type), _buttonID(buttonID)
    {

    }

    int getButtonID() const
    {
        return _buttonID;
    }

private:
    int _buttonID;
};

#endif // EVENT_HPP