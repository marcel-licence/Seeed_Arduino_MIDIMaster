//
// Created by Administrator on 25-4-1.
//

#ifndef AUDITIONMODE_H
#define AUDITIONMODE_H

#include "State.h"
#include "StateManager.h"
#include "Event.h"
#include "StateMachine.h"
#include "SAM2695Synth.h"

#define MaxTimeLimit 2000 //录制最大间隔，设置为2s

extern SAM2695Synth synth;

extern bool entryFlag;
extern bool channel_1_on_off_flag;
extern bool channel_2_on_off_flag;
extern bool channel_3_on_off_flag;
extern bool channel_4_on_off_flag;
extern bool drum_on_off_flag;

extern unsigned long btnPressStartTime;
extern uint8_t buttonPressCount;
extern bool isRecording;
extern uint8_t randomNote;


//AuditionMode
class AuditionMode :public State{
public:
	enum {ID = 1};
    AuditionMode();
	virtual void onEnter();
	virtual void onExit();
	//handle event
    virtual bool handleEvent(StateMachine* machine,Event* event);
	virtual int getID() const;
	virtual const char* getName() const;
};


#endif //BUTTONSTATE_H
