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

// Returns the type of the event stored in the _type member variable.
// This function is a getter for the _type field, which holds the type of the event.
// It does not modify the event object and is marked as a constant member function.
// 
// Returns:
//   - The type of the event as an EventType enum or class value.
EventType Event::getType() const
{
    return _type;
}

// Sets the type of the event to the specified EventType.
// This function is a setter for the _type member variable, allowing you to modify the event's type.
// 
// Parameters:
//   - type: The new EventType value to assign to the event's type.
// 
// This function modifies the _type member variable to the new value.
void Event::setType(EventType type)
{
    // Set the event type to the given type
    _type = type;
}


// Returns whether the event is currently in use, based on the _inUse member variable.
// This function is a getter for the _inUse field, which indicates if the event is active or being used.
// It does not modify the event object and is marked as a constant member function.
// 
// Returns:
//   - true if the event is in use (i.e., _inUse is true),
//   - false if the event is not in use (i.e., _inUse is false).
bool Event::isInUse() const
{
    // Return the value of _inUse to indicate if the event is currently in use
    return _inUse;
}


// Sets whether the event is currently in use based on the provided boolean value.
// This function is a setter for the _inUse member variable, allowing you to modify its value.
// 
// Parameters:
//   - inUse: A boolean value that specifies if the event is in use. 
//     - true: The event is considered to be in use.
//     - false: The event is not in use.
// 
// This function modifies the _inUse member variable to the new value.
void Event::setInUse(bool inUse)
{
    // Set the _inUse variable to the given value
    _inUse = inUse;
}


// Returns the timestamp of the event stored in the _timestamp member variable.
// This function is a getter for the _timestamp field, which holds the timestamp when the event occurred.
// It does not modify the event object and is marked as a constant member function.
//
// Returns:
//   - The timestamp of the event as an unsigned long value.
unsigned long Event::getTimestamp() const
{
    // Return the value of _timestamp to retrieve the timestamp of the event
    return _timestamp;
}

