//
// Created by Administrator on 25-4-9.
//

#ifndef BPMMODE_H
#define BPMMODE_H

#include "AuditionMode.h"

//BpmMode
class BpmMode :public State{
public:
	BpmMode();
	enum {ID = 2};
    virtual void onEnter();
	virtual void onExit();
	//handle event
    virtual bool handleEvent(StateMachine* machine,Event* event);
	virtual int getID() const;
	virtual const char* getName() const;;
};




#endif //BPMMODE_H
