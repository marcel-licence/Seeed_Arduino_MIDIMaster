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
    Event(EventType type = None);
    Event(EventType type, unsigned long timestamp);
    virtual ~Event();
    EventType getType() const;
    void setType(EventType type);
    bool isInUse() const;
    void setInUse(bool inUse);
    unsigned long getTimestamp() const;

private:
    EventType _type;
    unsigned long _timestamp;
    bool _inUse;
};

#endif // EVENT_HPP