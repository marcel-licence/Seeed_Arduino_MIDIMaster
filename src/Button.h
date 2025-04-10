//
// Created by Administrator on 25-4-7.
//

#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Event.h"


extern bool shortPressFlag_A;
extern bool longPressFlag_A;
extern bool releaseFlag_A;
extern bool shortPressFlag_B;
extern bool longPressFlag_B;
extern bool releaseFlag_B;
extern bool shortPressFlag_C;
extern bool longPressFlag_C;
extern bool releaseFlag_C;
extern bool shortPressFlag_D;
extern bool longPressFlag_D;
extern bool releaseFlag_D;

//
struct BtnState 
{
    int buttonState;
    int lastButtonState;
    unsigned long lastDebounceTime;
    unsigned long pressStartTime;
    bool longPressTriggered;
};

struct ButtonFlags 
{
    bool& shortPress;
    bool& longPress;
    bool& release;
    EventType shortPressType;
    EventType longPressType;
};

void initButtons(uint8_t pin);
void detectButtonEvents(uint8_t buttonPin, BtnState& button, bool& shortPressFlag, bool& releaseFlag,bool& longPressFlag);



#endif //BUTTON_H
