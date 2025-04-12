#pragma once

#include "Event.h"

class StateMachine;

class State
{
public:
    // Virtual destructor to ensure proper cleanup of derived class objects
    // when deleted via a base class pointer.
    virtual ~State() {}

    // Virtual function called when the state is entered.
    // Derived classes can override this to implement specific behavior when the state is entered.
    virtual void onEnter() {}

    // Virtual function called when the state is exited.
    // Derived classes can override this to implement specific behavior when the state is exited.
    virtual void onExit() {}

    // Pure virtual function to handle events.
    // Derived classes must implement this function to process events.
    // The function returns a boolean indicating whether the event was handled.
    // StateMachine* machine: Pointer to the state machine.
    // Event* event: Pointer to the event to be processed.
    virtual bool handleEvent(StateMachine* machine, Event* event) = 0;

    // Pure virtual function to return the unique ID of the state.
    // Derived classes must implement this function to return the state ID.
    virtual int getID() const = 0;

    // Pure virtual function to return the name of the state.
    // Derived classes must implement this function to return the state name as a string.
    virtual const char* getName() const = 0;
};
