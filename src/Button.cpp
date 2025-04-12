//
// Created by Administrator on 25-4-7.
//

#include "Button.h"

// Debounce time (milliseconds)
const unsigned long debounceDelay = 50;
// Long press duration (milliseconds)
const unsigned long longPressTime = 1000;

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

// Initializes the button pin with an internal pull-up resistor enabled.
// This ensures that the button pin will read HIGH when not pressed, and LOW when pressed.
// Parameters:
//   - pin: The pin number where the button is connected.
// For other platforms, you need to refer to the corresponding documentation.
//e.g
//  #define BUTTON_A_PIN 1  // Button 1 on XIAO_ESP32S3
//  #define BUTTON_B_PIN 2  // Button 2 on XIAO_ESP32S3
//  #define BUTTON_C_PIN 3  // Button 3 on XIAO_ESP32S3
//  #define BUTTON_D_PIN 4  // Button 4 on XIAO_ESP32S3
void initButtons(uint8_t pin)
{
    pinMode(pin,INPUT_PULLUP);
}

// Detects button events (short press, long press, and release) for a given button pin.
// This function reads the current button state, processes debounce logic, and updates flags 
// for short press, long press, and button release events based on the button's state and duration.
// It uses a debounce mechanism to avoid multiple readings for a single button press.
// Parameters:
//   - buttonPin: The pin number where the button is connected.
//   - button: A reference to the BtnState struct which holds the state of the button and timing information.
//   - shortPressFlag: A reference to a boolean flag indicating if a short press event has occurred.
//   - longPressFlag: A reference to a boolean flag indicating if a long press event has occurred.
//   - releaseFlag: A reference to a boolean flag indicating if the button has been released.
// The function does not return any values.
void detectButtonEvents(uint8_t buttonPin, BtnState& button, bool& shortPressFlag, bool& longPressFlag, bool& releaseFlag) {
    // Read button status
    int reading = digitalRead(buttonPin);

    // Detect if the button status has changed
    if (reading != button.lastButtonState) {
        button.lastDebounceTime = millis();
    }

    // Debounce processing
    if ((millis() - button.lastDebounceTime) > debounceDelay) {
        if (reading != button.buttonState) {
            button.buttonState = reading;

            if (button.buttonState == LOW) {
                // Button press
                button.pressStartTime = millis();
                button.longPressTriggered = false;
            } else {
                // Button release
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

    // Update the previous button status
    button.lastButtonState = reading;
}