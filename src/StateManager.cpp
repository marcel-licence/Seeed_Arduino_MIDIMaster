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

// Registers a state into the state manager's array of states.
// This function first checks if the provided state is valid (not nullptr) and if its ID is within the acceptable range.
// If a state with the same ID already exists, it deletes the old state before registering the new one.
// It logs the addition of the new state using its name and ID.
// Returns `true` if the state is successfully registered, or `false` if the state is invalid or if the ID is out of range.
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

// Retrieves a state by its index from the state manager's array of states.
// This function checks if the provided index is within a valid range. If the index is invalid (less than 0 or greater than or equal to MAX_STATES),
// it returns `nullptr`.
// Otherwise, it returns the state located at the specified index in the state array.
State* StateManager::getState(int index) const
{
    if(index < 0 || index >= MAX_STATES)
    {
        return nullptr;
    }
    State* state = _states[index - 1];
    return state;
}


// Returns the total number of states currently registered in the state manager.
// This function iterates through the state array and counts how many states are not `nullptr`.
// It returns the count of valid states (i.e., states that have been registered and are not null).
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



