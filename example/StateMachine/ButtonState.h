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

#define MaxTimeLimit 2000 // Record maximum interval, set to 2 seconds

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


//AuditionMode
class AuditionMode :public State{
public:
	enum {ID = 1};
    AuditionMode(){ }

    virtual void onEnter()
    {
	    Serial.println("enter AuditionMode");
    }

    virtual void onExit()
    {
	    Serial.println("exit AuditionMode");
    	drum_on_off_flag = false;
    }
    //handle event
    virtual bool handleEvent(StateMachine* machine,Event* event)
    {
        if(machine == nullptr || event == nullptr){
            return false;
        }

        switch(event->getType()){
            case EventType::APressed:{
					Serial.println("AuditionMode Button A Pressed");
					static uint8_t instrument = unit_synth_instrument_t::StringEnsemble2;
            		instrument++;
            		Serial.println("Instrument: " + String(instrument));
            		synth.setInstrument(0,CHANNEL_0,instrument);
            		synth.setPitch(instrument);
            		synth.setNoteOn(CHANNEL_0,synth.getPitch(),VELOCITY_MAX);
            		return true;
            };
            case EventType::BPressed:{
            		Serial.println("AuditionMode Button B Pressed");
            		synth.decreasePitch();
            		synth.setNoteOn(CHANNEL_0, synth.getPitch(), VELOCITY_MAX);
            		return true;
            };
            case EventType::CPressed:{
            		Serial.println("AuditionMode Button C Pressed");
            		synth.increasePitch();
            		synth.setNoteOn(CHANNEL_0, synth.getPitch(), VELOCITY_MAX);
            		return true;
            };
            case EventType::DPressed:{
            		Serial.println("AuditionMode Button D Pressed");
					drum_on_off_flag = !drum_on_off_flag;
            		return true;
            };
            case EventType::ALongPressed:{
            		Serial.println("AuditionMode Button A Long Pressed");
            		return true;
            };
            case EventType::BLongPressed:{
            		Serial.println("AuditionMode Button B Long Pressed");
            		synth.decreaseVelocity();
            		return true;
            };
            case EventType::CLongPressed:{
            		Serial.println("AuditionMode Button C Long Pressed");
            		synth.increaseVelocity();
            		return true;
            };
            case EventType::DLongPressed:{
            		Serial.println("AuditionMode Button D Long Pressed");
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
            		for(uint8_t i = CHANNEL_0;i<=CHANNEL_15;i++)
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
    virtual const char* getName() const {return "AuditionMode";}
};

//BpmMode
class BpmMode :public State{
public:
	enum {ID = 2};
    virtual void onEnter()
    {
	    Serial.println("enter BpmMode");
    }

    virtual void onExit()
    {
    	Serial.println("exit BpmMode");
    	drum_on_off_flag = false;
    }

    //handle event
    virtual bool handleEvent(StateMachine* machine,Event* event){
	    if(machine == nullptr || event == nullptr){
	        return false;
	    }

	    switch(event->getType()){
			case EventType::APressed:{
					Serial.println("BpmMode Button A  Pressed");
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
					Serial.println("BpmMode Button B  Pressed");
					synth.decreaseBpm();
					return true;
	        };
			case EventType::CPressed:{
					Serial.println("BpmMode Button C  Pressed");
					synth.increaseBpm();
					return true;
	        };
			case EventType::DPressed:{
					Serial.println("BpmMode Button D  Pressed");
					drum_on_off_flag = !drum_on_off_flag;
					return true;
	        };
			case EventType::ALongPressed:{
					Serial.println("BpmMode Button A Long Pressed");
					return true;
	        };
			case EventType::BLongPressed:{
					Serial.println("BpmMode Button B Long Pressed");
					synth.decreaseVelocity();
					return true;
	        };
			case EventType::CLongPressed:{
					Serial.println("BpmMode Button C Long Pressed");
					synth.increaseVelocity();
					return true;
	        };
			case EventType::DLongPressed:{
				Serial.println("BpmMode Button D Long Pressed");
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
		    case EventType::None:{
				entryFlag = true;
		    };
			default:
				return false;
	    }
	}
    virtual int getID() const {return ID;}
    virtual const char* getName() const {return "BpmMode";};
};

//TrackMode
class TrackMode :public State{
public:
	enum {ID = 3};
	virtual void onEnter()
	{
		Serial.println("enter TrackMode");
	}

	virtual void onExit()
	{
		Serial.println("exit TrackMode");
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
				Serial.println("TrackMode Button A Pressed");
				channel_1_on_off_flag = !channel_1_on_off_flag;
				return true;
		};
		case EventType::BPressed:{
	    		Serial.println("TrackMode Button B Pressed");
				channel_2_on_off_flag = !channel_2_on_off_flag;
				return true;
		};
		case EventType::CPressed:{
               Serial.println("TrackMode Button C Pressed");
				channel_3_on_off_flag = !channel_3_on_off_flag;
				return true;
		};
		case EventType::DPressed:{
    			Serial.println("TrackMode Button D Pressed");
				channel_4_on_off_flag = !channel_4_on_off_flag;
				return true;
		};
		case EventType::ALongPressed:{
				Serial.println("TrackMode Button A Long Pressed");
				return true;
		};
		case EventType::BLongPressed:{
    			Serial.println("TrackMode Button B Long Pressed");
				synth.decreaseVelocity();
				return true;
		};
		case EventType::CLongPressed:{
    			Serial.println("TrackMode Button C Long Pressed");
				synth.increaseVelocity();
				return true;
		};
		case EventType::DLongPressed:{
				Serial.println("TrackMode Button D Long Pressed");
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
		case EventType::None:{
				entryFlag = true;
		};
		default:
			return false;
		}
	}
	virtual int getID() const {return ID;}
	virtual const char* getName() const {return "TrackMode";};
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
