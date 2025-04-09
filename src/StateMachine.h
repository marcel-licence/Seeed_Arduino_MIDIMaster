//
// Created by Administrator on 25-4-1.
//

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.h"

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

private:
    State* _currentState;
    State* _previousState;
    State* _errorState;
    ErrorHandler _errorHandler;
    bool _transitioningInProgress;
};



#endif //STATEMACHINE_H
