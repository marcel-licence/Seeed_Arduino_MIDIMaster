//
// Created by Administrator on 25-4-1.
//

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "State.hpp"
#include <Arduino.h>

#define MAX_STATES 10

//状态管理器
class StateManager {
public:
    ~StateManager();
    //获取单例
    static StateManager* getInstance();
    //释放单例
    static void releaseInstance();
    //注册状态
    bool registerState(State* state);
    //获取状态
    State* getState(int index) const;
    //状态数量
    int getStateCount() const;

private:
    //构造私有化
    StateManager();

private:
    static StateManager* _instance;
    State* _states[MAX_STATES];
};



#endif //STATEMANAGER_H
