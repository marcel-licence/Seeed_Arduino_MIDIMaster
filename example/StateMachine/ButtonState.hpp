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

extern SAM2695Synth synth;

bool entryFlag = true;
bool channel_1_on_off_flag = false;
bool channel_2_on_off_flag = false;
bool channel_3_on_off_flag = false;
bool channel_4_on_off_flag = false;
bool drum_on_off_flag = false;

//mode 1
class ButtonState1 :public State{
public:
	enum {ID = 1};
    ButtonState1(){ }

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
            case EventType::BtnAPressed:{
				Serial.println("Mode 1 Button A Pressed");
            		srand(static_cast<unsigned int>(time(0)));
            		int randomNumber = rand() % 128;
            		synth.setInstrument(0,0,randomNumber);
            		return true;
            };
            case EventType::BtnBPressed:{
            		Serial.println("Mode 1 Button B Pressed");
            		synth.decreasePitch();
            		return true;
            };
            case EventType::BtnCPressed:{
            		Serial.println("Mode 1 Button C Pressed");
            		synth.increasePitch();
            		return true;
            };
            case EventType::BtnDPressed:{
            		Serial.println("Mode 1 Button D Pressed");
            		return true;
            };
            case EventType::BtnALongPressed:{
            		Serial.println("Mode 1 Button A Long Pressed");
            		return true;
            };
            case EventType::BtnBLongPressed:{
            		Serial.println("Mode 1 Button B Long Pressed");
            		synth.decreaseVelocity();
            		return true;
            };
            case EventType::BtnCLongPressed:{
            		Serial.println("Mode 1 Button C Long Pressed");
            		synth.increaseVelocity();
            		return true;
            };
            case EventType::BtnDLongPressed:{
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

        	case EventType::NoEvent:{
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
class ButtonState2 :public State{
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
			case EventType::BtnAPressed:{
					Serial.println("Mode 2 Button A  Pressed");
					srand(static_cast<unsigned int>(time(0)));
					int randomNumber = rand() % 128;
					synth.setInstrument(0,0,randomNumber);
					return true;
	        };
			case EventType::BtnBPressed:{
					Serial.println("Mode 2 Button B  Pressed");
					synth.decreaseBpm();
					return true;
	        };
			case EventType::BtnCPressed:{
					Serial.println("Mode 2 Button C  Pressed");
					synth.increaseBpm();
					return true;
	        };
			case EventType::BtnDPressed:{
					Serial.println("Mode 2 Button D  Pressed");
					drum_on_off_flag = !drum_on_off_flag;
					return true;
	        };
			case EventType::BtnALongPressed:{
					Serial.println("Mode 2 Button A Long Pressed");
					return true;
	        };
			case EventType::BtnBLongPressed:{
					Serial.println("Mode 2 Button B Long Pressed");
					synth.decreaseVelocity();
					return true;
	        };
			case EventType::BtnCLongPressed:{
					Serial.println("Mode 2 Button C Long Pressed");
					synth.increaseVelocity();
					return true;
	        };
			case EventType::BtnDLongPressed:{
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
class ButtonState3 :public State{
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

		case EventType::BtnAPressed:{
				Serial.println("Mode 3 Button A Pressed");
				channel_1_on_off_flag = !channel_1_on_off_flag;
				return true;
		};
		case EventType::BtnBPressed:{
	    		Serial.println("Mode 3 Button B Pressed");
				channel_2_on_off_flag = !channel_2_on_off_flag;
				return true;
		};
		case EventType::BtnCPressed:{
               Serial.println("Mode 3 Button C Pressed");
				channel_3_on_off_flag = !channel_3_on_off_flag;
				return true;
		};
		case EventType::BtnDPressed:{
    			Serial.println("Mode 3 Button D Pressed");
				channel_4_on_off_flag = !channel_4_on_off_flag;
				return true;
		};
		case EventType::BtnALongPressed:{
				Serial.println("Mode 3 Button A Long Pressed");
				return true;
		};
		case EventType::BtnBLongPressed:{
    			Serial.println("Mode 3 Button B Long Pressed");
				synth.decreaseVelocity();
				return true;
		};
		case EventType::BtnCLongPressed:{
    			Serial.println("Mode 3 Button C Long Pressed");
				synth.increaseVelocity();
				return true;
		};
		case EventType::BtnDLongPressed:{
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
