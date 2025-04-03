/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#include "Button.h"
#include <Arduino.h>



__Button::__Button(uint8_t pin, uint16_t debounce_ms)
:  pin_(pin)
,  delay_(debounce_ms)
,  state_(HIGH)
,  ignoreUntil_(0)
,  hasChanged_(false)
,  pressedTime_(0)
,  longPressThreshold_(700)
,  longPressFlag_(false)
{
	pin_ = pin;
    pinMode(pin, INPUT_PULLUP);
}

void __Button::begin(){
	pinMode(pin_, INPUT_PULLUP);
}

// 
// public methods
//

// has the button been toggled from on -> off, or vice versa
BtnAct __Button::toggled(){
	if (has_changed()) {
		return BtnAct::Toggled;
	}
	return BtnAct::None;
}

bool __Button::read(){
	// ignore pin changes until after this delay time
	if (ignoreUntil_ > millis()){
		// ignore any changes during this period
	}
	
	// pin has changed 
	else if (digitalRead(pin_) != state_){
		ignoreUntil_ = millis() + delay_;
		state_ = !state_;
		hasChanged_ = true;
	}
	
	return state_;
}

// mostly internal, tells you if a button has changed after calling the read() function
bool __Button::has_changed(){
	if (hasChanged_){
		hasChanged_ = false;
		return true;
	}
	return false;
}

// has the button gone from off -> on
BtnAct __Button::pressed(){
	if (read() == LOW && has_changed()) {
		return BtnAct::Pressed;
	}
	return BtnAct::None;
}

// has the button gone from on -> off
BtnAct __Button::released(){
	if (read() == HIGH && has_changed()) {
		return BtnAct::Released;
	}
	return BtnAct::None;
}

// has the button detect long press
BtnAct __Button::longPressed(){
	// If the button is pressed, check if it has been pressed for longer than the threshold
	if (read() == PRESSED) {
		if (pressedTime_ == 0) {
			// Record the time when the button is first pressed
			pressedTime_ = millis();
		}
		// Check if the button has been pressed long enough
		if (millis() - pressedTime_ >= longPressThreshold_ ) {
			if(longPressFlag_ == false){
				longPressFlag_ = true;
				return BtnAct::LongPressed;  // Long press detected
			}
			return BtnAct::None;  // Long press detected, but already reported
		}
	} else {
		// Reset the pressed_time when the button is released
		if(longPressFlag_ ){
			longPressFlag_ = false;
		}
		pressedTime_ = 0;
	}
	return BtnAct::None;   // No long press
}

uint8_t __Button::getPin() const
{
    return pin_;
}

