//
// Created by Administrator on 25-4-7.
//

#include "Button.h"

// Debounce time (milliseconds)
const unsigned long debounceDelay = 50;
// Long press time (milliseconds)
const unsigned long longPressTime = 1000;

// Event flags
bool shortPressFlag_A = false;
bool longPressFlag_A = false;
bool releaseFlag_A = false;
bool shortPressFlag_B = false;
bool longPressFlag_B = false;
bool releaseFlag_B = false;
bool shortPressFlag_C = false;
bool longPressFlag_C = false;
bool releaseFlag_C = false;
bool shortPressFlag_D = false;
bool longPressFlag_D = false;
bool releaseFlag_D = false;

void initButtons()
{
    pinMode(BUTTON_A_PIN,INPUT_PULLUP);
    pinMode(BUTTON_B_PIN,INPUT_PULLUP);
    pinMode(BUTTON_C_PIN,INPUT_PULLUP);
    pinMode(BUTTON_D_PIN,INPUT_PULLUP);
}

 // Encapsulate key detection function
void detectButtonEvents(uint8_t buttonPin, BtnState& button, bool& shortPressFlag, bool& longPressFlag, bool& releaseFlag) {
    // Read key status
    int reading = digitalRead(buttonPin);

    // Detect if key status has changed
    if (reading != button.lastButtonState) {
        button.lastDebounceTime = millis();
    }

     // Debounce processing
    if ((millis() - button.lastDebounceTime) > debounceDelay) {
        if (reading != button.buttonState) {
            button.buttonState = reading;

            if (button.buttonState == LOW) {
                // Key press
                button.pressStartTime = millis();
                button.longPressTriggered = false;
            } else {
                // Key release
                unsigned long pressDuration = millis() - button.pressStartTime;
                if (!button.longPressTriggered && pressDuration < longPressTime) {
                    // Short press event
                    shortPressFlag = true;
                }
                releaseFlag = true;
            }
        }
    }

   // Detect long press event
    if (button.buttonState == LOW && (millis() - button.pressStartTime) >= longPressTime) {
        if (!button.longPressTriggered) {
            longPressFlag = true;
            button.longPressTriggered = true;
        }
    }

     // Update the previous key status
    button.lastButtonState = reading;
}