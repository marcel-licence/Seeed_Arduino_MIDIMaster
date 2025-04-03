//
// Created by Administrator on 25-4-1.
//

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.hpp"

using ErrorHandler = void (*)(int errorCode,const char* errorMsg);

//状态机
class StateMachine {
public:
    StateMachine();
    ~StateMachine();

    //初始化状态机
    bool init(State* initialState, State* errorState);
    //事件处理
    bool handleEvent(const Event* event);
    //状态转化
    bool changeState(State* newState);
    //返回前一个状态
    bool goToPreviousState();
    //设置错误处理器
    void setErrorHandler(ErrorHandler errorHandler);
    //触发错误处理
    void handleError(int errorCode,const char* errorMsg);
    //获取当前状态
    State* getCurrentState();

private:
    State* _currentState;
    State* _previousState;
    State* _errorState;
    ErrorHandler _errorHandler;
    bool _transitioningInProgress;
};



#endif //STATEMACHINE_H
