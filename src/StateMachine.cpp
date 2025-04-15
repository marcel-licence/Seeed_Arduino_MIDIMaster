//
// Created by Administrator on 25-4-1.
//

#include "StateMachine.h"

StateMachine::StateMachine()
: _currentState(nullptr)
, _errorHandler(nullptr)
, _previousState(nullptr)
, _errorState(nullptr)
, _transitioningInProgress(false)
, _currentEventIndex(0)
{
    reset();
}

StateMachine::~StateMachine()
{

}

// Initializes the state machine with the given initial and error states.
// This function sets the current state to the provided initial state and the error state to the provided error state.
// It then attempts to invoke the onEnter() method of the initial state to prepare it for execution.
// If any exception occurs during the onEnter() method call, the error is handled, and the function returns false.
// No parameters are required for this function other than the initial and error state pointers.
// Returns true if initialization is successful, false otherwise.
bool StateMachine::init(State* initialState, State* errorState)
{
    if(initialState == nullptr)
    {
        return false;
    }

    _currentState = initialState;
    _errorState = errorState;

    _currentState->onEnter();
    return true;
}


// Handles an incoming event by delegating the event processing to the current state.
// This function first checks if the current state or the event is null, returning false if either is invalid.
// If a state transition is already in progress, it prevents reprocessing of the event to avoid potential issues.
// The event is passed to the current state's `handleEvent` method, and the function tracks the transition state to avoid concurrency issues.
// If an exception occurs during event processing, the error is handled, and false is returned.
// Returns true if the event is successfully handled, false otherwise.
bool StateMachine::handleEvent(const Event* event)
{
    if(!_currentState || !event)
    {
        return false;
    }

    // Prevent processing the event again 
    //when the state transition is triggered during event handling.
    if(_transitioningInProgress)
    {
        return false;
    }

    _transitioningInProgress = true;
    bool handled = _currentState->handleEvent(this,(Event*)event);
    _transitioningInProgress = false;
    return handled;
}


// Changes the current state of the state machine to the new state, ensuring a smooth transition.
// The function checks if the new state is valid and different from the current state; 
// it returns false if the new state is invalid or the same as the current state.
// The current state is exited first, and the previous state is saved before transitioning to the new state.
// Once the transition is made, the new state is entered.
// If an exception occurs during the state change, the error is handled and false is returned.
// Returns true if the state transition is successful, false otherwise.
bool StateMachine::changeState(State* newState)
{
    if (!newState || newState == _currentState) 
    {
        return false;
    }

    // exit current state
    if (_currentState) 
    {
        _currentState->onExit();
    }

    //Save the previous state for return
    _previousState = _currentState;
    _currentState = newState;

    // Enter the new state
    _currentState->onEnter();
    return true;
}


// Transitions the state machine to the previous state, if available.
// The function checks if there is a previous state saved; if not, it returns false.
// If a previous state exists, the state machine transitions to that state using the `changeState` method.
// Returns true if the transition to the previous state is successful, false if no previous state exists.
bool StateMachine::goToPreviousState()
{
    if (!_previousState)
    {
        return false;
    }

    return changeState(_previousState);
}


// Sets the custom error handler for the state machine.
// This function allows the user to provide a custom error handler function 
// that will be used to handle errors during state transitions or other processes.
// The provided `errorHandler` will be stored and invoked when an error occurs in the state machine.
// No return value.
void StateMachine::setErrorHandler(ErrorHandler errorHandler)
{
    _errorHandler = errorHandler;
}


// Handles errors by invoking the error handler and transitioning to the error state if necessary.
// This function first checks if a custom error handler is set; if so, it calls the handler with the provided 
// error code and message.
// After handling the error, the function checks if an error state is defined and whether the current state 
// is not already the error state. If both conditions are met, the state machine transitions to the error state.
// No return value.
void StateMachine::handleError(int errorCode, const char* errorMsg)
{
    // Call the error handler
    if (_errorHandler)
    {
        _errorHandler(errorCode, errorMsg);
    }

    // Switch to the error state if there is an error state
    if (_errorState && _currentState != _errorState)
    {
        changeState(_errorState);
    }
}


// Returns the current state of the state machine.
// This function provides access to the state machine's current state object.
// It allows other components to retrieve the current state without modifying it.
// Returns a pointer to the current state object (nullptr if no state is set).
State* StateMachine::getCurrentState()
{
    return _currentState;
}


// Retrieves an available event from the event array and sets its type.
// This function iterates through the event array to find an event that is not in use.
// Once a free event is found, it sets the event's type and marks it as in use.
// Returns a pointer to the event if one is found, or nullptr if no free events are available.
Event* StateMachine::getEvent(EventType type)
{
    for (int i = 0; i < EVENT_ARR_SIZE; i++) 
    {
        if (!_eventArr[i].isInUse()) 
        {
            _eventArr[i].setType(type);
            _eventArr[i].setInUse(true);
            return &_eventArr[i];
        }
    }
    return nullptr;
}


// Recycles an event by marking it as no longer in use.
// This function takes an event and, if it is not nullptr, marks it as not in use, 
// making it available for reuse in the future.
// The function ensures proper memory management by resetting the event's status.
void StateMachine::recycleEvent(Event* event)
{
    if (event) 
    {
        event->setInUse(false);
    }
}


// Resets the state machine's event array and current event index.
// This function iterates through the event array and resets each event's type to `None`
// and marks it as not in use, making all events available for reuse.
// It also resets the current event index to 0, preparing the state machine for a fresh cycle.
void StateMachine::reset()
{
    for (int i = 0; i < EVENT_ARR_SIZE; i++) 
    {
        _eventArr[i].setType(EventType::None);
        _eventArr[i].setInUse(false);
    }
    _currentEventIndex = 0;
}

