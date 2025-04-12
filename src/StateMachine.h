//
// Created by Administrator on 25-4-1.
//

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.h"

#define EVENT_ARR_SIZE 3

using ErrorHandler = void (*)(int errorCode,const char* errorMsg);

class StateMachine {
public:
    StateMachine();
    ~StateMachine();

    //init state mechine
    bool init(State* initialState, State* errorState);
    //handle event 
    bool handleEvent(const Event* event);
    //toggle to next state
    bool changeState(State* newState);
    //toggle to previous state
    bool goToPreviousState();
    //set error event handle
    void setErrorHandler(ErrorHandler errorHandler);
    //handle error
    void handleError(int errorCode,const char* errorMsg);
    //get current state
    State* getCurrentState();
    // get event from event arr
    Event* getEvent(EventType type);
    // set the event type is none
    void recycleEvent(Event* event);
    // reset event arr
    void reset();

    private:
    State* _currentState;
    State* _previousState;
    State* _errorState;
    Event _eventArr[EVENT_ARR_SIZE];
    int _currentEventIndex;
    ErrorHandler _errorHandler;
    bool _transitioningInProgress;
};



#endif //STATEMACHINE_H
