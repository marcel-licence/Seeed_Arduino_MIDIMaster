//
// Created by Administrator on 25-4-1.
//

#include "StateManager.h"

StateManager* StateManager::_instance = nullptr;

StateManager::StateManager()
{
    // 初始化状态数组
    for (int i = 0; i < MAX_STATES; ++i)
    {
        _states[i] = 0;
    }
}

StateManager::~StateManager()
{
    // 清理状态（如果它们是动态分配的）
    for (int i = 0; i < MAX_STATES; ++i) {
        delete _states[i];
        _states[i] = 0;
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

    // 如果状态已经存在，则先删除它
    if(_states[id-1])
    {
        delete _states[id-1];
    }
    //替换
    _states[id-1] = state;
    char name[100];  // 用一个足够大的字符数组来存储结果
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


