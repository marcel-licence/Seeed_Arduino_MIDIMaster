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

    // 防止在事件处理中触发状态转换时再次处理事件
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
        // 退出当前状态
        if (_currentState) {
            _currentState->onExit();
        }

        // 保存前一个状态用于返回
        _previousState = _currentState;
        _currentState = newState;

        // 进入新状态
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
    // 调用错误处理器
    if (_errorHandler) {
        _errorHandler(errorCode, errorMsg);
    }

    // 如果有错误状态，切换到错误状态
    if (_errorState && _currentState != _errorState) {
        changeState(_errorState);
    }
}

State* StateMachine::getCurrentState()
{
    return _currentState;
}
