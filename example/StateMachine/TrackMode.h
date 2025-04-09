//
// Created by Administrator on 25-4-9.
//

#ifndef TRACKMODE_H
#define TRACKMODE_H

#include "AuditionMode.h"

//TrackMode
class TrackMode :public State{
public:
	enum {ID = 3};
	TrackMode();
	virtual void onEnter();

	virtual void onExit();
	//handle event
	virtual bool handleEvent(StateMachine* machine,Event* event);

	virtual int getID() const;
	virtual const char* getName() const;;
};


#endif //TRACKMODE_H
