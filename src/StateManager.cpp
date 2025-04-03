//
// Created by Administrator on 25-4-1.
//

#include "StateManager.h"

StateManager* StateManager::_instance = nullptr;

StateManager::StateManager()
{
    // init state array
    for (int i = 0; i < MAX_STATES; ++i)
    {
        _states[i] = nullptr;
    }
}

StateManager::~StateManager()
{
    // reset state array
    for (int i = 0; i < MAX_STATES; ++i) {
        delete _states[i];
        _states[i] = nullptr;
    }
}

StateManager* StateManager::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new StateManager();
    }
    return _instance;
}

void StateManager::releaseInstance()
{
    delete _instance;
    _instance = nullptr;
}

bool StateManager::registerState(State* state)
{
    if(state == nullptr)
    {
        return false;
    }

    int id = state->getID();
    if(id < 0 || id >= MAX_STATES)
    {
        return false;
    }

    // if existed! delete it!
    if(_states[id-1])
    {
        delete _states[id-1];
    }
    _states[id-1] = state;
    char name[100];  // LOG
    sprintf(name, "add state : %s,%d", state->getName(), id);
    Serial.println(name);
    return true;
}

State* StateManager::getState(int index) const
{
    if(index < 0 || index >= MAX_STATES)
    {
        return nullptr;
    }
    State* state = _states[index - 1];
    return state;
}

int StateManager::getStateCount() const
{
    int count = 0;
    for (int i = 0; i < MAX_STATES; ++i)
    {
        if(_states[i])
        {
            ++count;
        }
    }
    return count;
}


