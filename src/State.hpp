#pragma once

#include "Event.hpp"

class StateMachine;

class State
{
public:
    virtual ~State() {}
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual bool handleEvent(StateMachine* machine,Event* event) = 0;
    virtual int getID() const = 0;
    virtual const char* getName() const = 0;
};