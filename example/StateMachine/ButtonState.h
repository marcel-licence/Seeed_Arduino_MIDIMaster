//
// Created by Administrator on 25-4-1.
//

#ifndef BUTTONSTATE_H
#define BUTTONSTATE_H

#include "State.hpp"
#include "StateManager.h"
#include "Event.hpp"
#include "StateMachine.h"
#include "SAM2695Synth.h"

#define MaxTimeLimit 2000 //录制最大间隔，设置为2s

extern SAM2695Synth synth;

bool entryFlag = true;
bool channel_1_on_off_flag = false;
bool channel_2_on_off_flag = false;
bool channel_3_on_off_flag = false;
bool channel_4_on_off_flag = false;
bool drum_on_off_flag = false;

unsigned long btnPressStartTime  = 0;
uint8_t buttonPressCount = 0;
bool isRecording = false;
uint8_t randomNote = 0;


//mode 1
class State1 :public State{
public:
	enum {ID = 1};
    State1(){ }

    virtual void onEnter()
    {
	    Serial.println("enter Mode 1");
    }

    virtual void onExit()
    {
	    Serial.println("exit Mode 1");
    }
    //handle event
    virtual bool handleEvent(StateMachine* machine,Event* event)
    {
        if(machine == nullptr || event == nullptr){
            return false;
        }

        switch(event->getType()){
            case EventType::APressed:{
					Serial.println("Mode 1 Button A Pressed");
            		srand(static_cast<unsigned int>(time(0)));
            		randomNote = rand() % 128;
            		synth.setInstrument(0,CHANNEL_10,randomNote);
            		synth.setNoteOn(CHANNEL_10,randomNote,80);
            		return true;
            };
            case EventType::BPressed:{
            		Serial.println("Mode 1 Button B Pressed");
            		synth.decreasePitch();
            		return true;
            };
            case EventType::CPressed:{
            		Serial.println("Mode 1 Button C Pressed");
            		synth.increasePitch();
            		return true;
            };
            case EventType::DPressed:{
            		Serial.println("Mode 1 Button D Pressed");
					drum_on_off_flag = !drum_on_off_flag;
            		return true;
            };
            case EventType::ALongPressed:{
            		Serial.println("Mode 1 Button A Long Pressed");
            		return true;
            };
            case EventType::BLongPressed:{
            		Serial.println("Mode 1 Button B Long Pressed");
            		synth.decreaseVelocity();
            		return true;
            };
            case EventType::CLongPressed:{
            		Serial.println("Mode 1 Button C Long Pressed");
            		synth.increaseVelocity();
            		return true;
            };
            case EventType::DLongPressed:{
            		Serial.println("Mode 1 Button D Long Pressed");
					//next mode index
					int index = 2;
		            State* nextState = StateManager::getInstance()->getState(index);
		            if(nextState != nullptr){
            			machine->changeState(nextState);
            			entryFlag = true;
            			return true;
		            }
		            entryFlag = false;
		            return true;
            }
		    case EventType::BtnReleased:{
            		for(uint8_t i = 0;i<16;i++)
            		{
            			synth.setAllNotesOff(i);
            		}
            		entryFlag = true;
		    }
		    default:
					return false;
		    }
    }
    virtual int getID() const {return ID;}
    virtual const char* getName() const {return "ButtonStateOne";}
};

//mode 2
class State2 :public State{
public:
	enum {ID = 2};
    virtual void onEnter()
    {
	    Serial.println("enter Mode 2");
    }

    virtual void onExit()
    {
    	Serial.println("exit Mode 2");
    }

    //handle event
    virtual bool handleEvent(StateMachine* machine,Event* event){
	    if(machine == nullptr || event == nullptr){
	        return false;
	    }

	    switch(event->getType()){
			case EventType::APressed:{
					Serial.println("Mode 2 Button A  Pressed");
					//first press
					if(!isRecording)
					{
						btnPressStartTime = millis();		//record the start time
						buttonPressCount = 1;
						isRecording = true;
					}
					else
					{
						if (millis() - btnPressStartTime <= MaxTimeLimit)
						{
							buttonPressCount++;
						}
						else
						{
							isRecording = false;
							buttonPressCount = 0;
						}
					}
					if (buttonPressCount == 4) {
						//calculate the bpm
						int bpm = BASIC_TIME / (millis() - btnPressStartTime);
						synth.setBpm(bpm);
						Serial.println("BPM: " + String(bpm));
						isRecording = false;
						btnPressStartTime = 0;
					}
					return true;
	        };
			case EventType::BPressed:{
					Serial.println("Mode 2 Button B  Pressed");
					synth.decreaseBpm();
					return true;
	        };
			case EventType::CPressed:{
					Serial.println("Mode 2 Button C  Pressed");
					synth.increaseBpm();
					return true;
	        };
			case EventType::DPressed:{
					Serial.println("Mode 2 Button D  Pressed");
					drum_on_off_flag = !drum_on_off_flag;
					return true;
	        };
			case EventType::ALongPressed:{
					Serial.println("Mode 2 Button A Long Pressed");
					return true;
	        };
			case EventType::BLongPressed:{
					Serial.println("Mode 2 Button B Long Pressed");
					synth.decreaseVelocity();
					return true;
	        };
			case EventType::CLongPressed:{
					Serial.println("Mode 2 Button C Long Pressed");
					synth.increaseVelocity();
					return true;
	        };
			case EventType::DLongPressed:{
				Serial.println("Mode 2 Button D Long Pressed");
				if(entryFlag == false)
				{
					return false;
				}
				int index = 3;
				State* nextState = StateManager::getInstance()->getState(index);
				if(nextState != nullptr){
					machine->changeState(nextState);
					entryFlag = true;
					return true;
				}
				entryFlag = false;
				return true;
	        }
		    case EventType::NoEvent:{
				entryFlag = true;
		    };
			default:
				return false;
	    }
	}
    virtual int getID() const {return ID;}
    virtual const char* getName() const {return "ButtonStateTwo";};
};

//mode 3
class State3 :public State{
public:
	enum {ID = 3};
	virtual void onEnter()
	{
		Serial.println("enter Mode 3");
	}

	virtual void onExit()
	{
		Serial.println("exit Mode 3");
		channel_1_on_off_flag = false;
		channel_2_on_off_flag = false;
		channel_3_on_off_flag = false;
		channel_4_on_off_flag = false;
	}
	//handle event
	virtual bool handleEvent(StateMachine* machine,Event* event){
		if(machine == nullptr || event == nullptr){
			return false;
		}

		switch(event->getType()){

		case EventType::APressed:{
				Serial.println("Mode 3 Button A Pressed");
				channel_1_on_off_flag = !channel_1_on_off_flag;
				return true;
		};
		case EventType::BPressed:{
	    		Serial.println("Mode 3 Button B Pressed");
				channel_2_on_off_flag = !channel_2_on_off_flag;
				return true;
		};
		case EventType::CPressed:{
               Serial.println("Mode 3 Button C Pressed");
				channel_3_on_off_flag = !channel_3_on_off_flag;
				return true;
		};
		case EventType::DPressed:{
    			Serial.println("Mode 3 Button D Pressed");
				channel_4_on_off_flag = !channel_4_on_off_flag;
				return true;
		};
		case EventType::ALongPressed:{
				Serial.println("Mode 3 Button A Long Pressed");
				return true;
		};
		case EventType::BLongPressed:{
    			Serial.println("Mode 3 Button B Long Pressed");
				synth.decreaseVelocity();
				return true;
		};
		case EventType::CLongPressed:{
    			Serial.println("Mode 3 Button C Long Pressed");
				synth.increaseVelocity();
				return true;
		};
		case EventType::DLongPressed:{
				Serial.println("Mode 3 Button D Long Pressed");
				if(entryFlag == false)
					return false;
				int index = 1;
				State* nextState = StateManager::getInstance()->getState(index);
				if(nextState != nullptr){
					machine->changeState(nextState);
					entryFlag = true;
					return true;
				}
				return true;
		}
		case EventType::NoEvent:{
				entryFlag = true;
		};
		default:
			return false;
		}
	}
	virtual int getID() const {return ID;}
	virtual const char* getName() const {return "ButtonStateThree";};
};


//error mode
class ErrorState :public State
{
public:
    enum{ID = 100};

    ErrorState():_errorCode(0),_errorMsg("unknown error"){}
    void setError(int code,const char* msg)
    {
        _errorCode = code;
        _errorMsg = msg;
    }

    virtual void onEnter()
    {

    }

    virtual void onExit()
    {

    }
    virtual bool handleEvent(StateMachine* machine,Event* event)
    {
        if(machine == nullptr || event == nullptr)
        {
            return false;
        }
        return true;
    }
    virtual int getID() const {return ID;}
    virtual const char* getName() const {return "Error";};
private:
    int _errorCode;
    const char* _errorMsg;
};




#endif //BUTTONSTATE_H
