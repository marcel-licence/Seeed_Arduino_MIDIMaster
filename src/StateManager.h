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
    //get instance
    static StateManager* getInstance();
    //release instance
    static void releaseInstance();
    //regist state
    bool registerState(State* state);
    //get state by stateID
    State* getState(int index) const;
    //get state count
    int getStateCount() const;

private:
    //constructor private
    StateManager();

private:
    static StateManager* _instance;
    State* _states[MAX_STATES];
};



#endif //STATEMANAGER_H
