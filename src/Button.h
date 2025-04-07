//
// Created by Administrator on 25-4-7.
//

#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define BUTTON_A_PIN 1
#define BUTTON_B_PIN 2
#define BUTTON_C_PIN 3
#define BUTTON_D_PIN 4

enum BtnAct
{
    None,
    ShortPress,
    LongPress,
    Release
};

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

struct BtnState {
    int buttonState;
    int lastButtonState;
    unsigned long lastDebounceTime;
    unsigned long pressStartTime;
    bool longPressTriggered;
};

void initButtons();
// void detectButtonEvents(uint8_t buttonPin, BtnState& button, enum BtnAct& act);
void detectButtonEvents(uint8_t buttonPin, BtnState& button, bool& shortPressFlag, bool& releaseFlag,bool& longPressFlag);



#endif //BUTTON_H
