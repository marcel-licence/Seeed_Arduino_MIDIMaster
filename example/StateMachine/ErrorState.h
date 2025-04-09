//
// Created by Administrator on 25-4-9.
//

#ifndef ERRORSTATE_H
#define ERRORSTATE_H

#include "AuditionMode.h"

//error mode
class ErrorState :public State
{
public:
    enum{ID = 100};
    ErrorState();
    void setError(int code,const char* msg);
    virtual void onEnter();
    virtual void onExit();
    virtual bool handleEvent(StateMachine* machine,Event* event);
    virtual int getID() const;
    virtual const char* getName() const;;
private:
    int _errorCode;
    const char* _errorMsg;
};




#endif //ERRORSTATE_H
