//
// Created by Administrator on 25-4-1.
//

#include "StateMachine.h"

StateMachine::StateMachine()
: _currentState(nullptr)
, _errorHandler(nullptr)
, _previousState(nullptr)
, _transitioningInProgress(false)
{

}

StateMachine::~StateMachine()
{

}

bool StateMachine::init(State* initialState, State* errorState)
{
    if(initialState == nullptr)
    {
        return false;
    }

    _currentState = initialState;
    _errorState = errorState;

    try
    {
        _currentState->onEnter();
        return true;
    }
    catch(...)
    {
        handleError(1,"Exception in initial state onEnter()");
        return false;
    }
}

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

    try
    {
        _transitioningInProgress = true;
        bool handled = _currentState->handleEvent(this,(Event*)event);
        _transitioningInProgress = false;
        return handled;
    }
    catch(...)
    {
        _transitioningInProgress = false;
        handleError(2,"Exception in state handleEvent()");
        return false;
    }
}

bool StateMachine::changeState(State* newState)
{
    if (!newState || newState == _currentState) {
        return false;
    }

    try {
        // exit current state
        if (_currentState) {
            _currentState->onExit();
        }

        //Save the previous state for return
        _previousState = _currentState;
        _currentState = newState;

        // Enter the new state
        _currentState->onEnter();
        return true;
    } catch (...) {
        handleError(3, "Exception during state transition");
        return false;
    }
}

bool StateMachine::goToPreviousState()
{
    if (!_previousState) {
        return false;
    }

    return changeState(_previousState);
}

void StateMachine::setErrorHandler(ErrorHandler errorHandler)
{
    _errorHandler = errorHandler;
}

void StateMachine::handleError(int errorCode, const char* errorMsg)
{
    // Call the error handler
    if (_errorHandler) {
        _errorHandler(errorCode, errorMsg);
    }

    // Switch to the error state if there is an error state
    if (_errorState && _currentState != _errorState) {
        changeState(_errorState);
    }
}

State* StateMachine::getCurrentState()
{
    return _currentState;
}
