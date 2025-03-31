/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#ifndef Button_h
#define Button_h
#include "Arduino.h"

enum BtnAct
{
	none = 0,
	toggled,
	pressed ,
	released,
	longPressed
};

class __Button
{
public:
	__Button(uint8_t pin, uint16_t debounce_ms = 100);
	void begin();
	bool read();
	bool has_changed();
	BtnAct toggled();
	BtnAct pressed();
	BtnAct released();
	BtnAct longPressed();

	const static bool PRESSED = LOW;
	const static bool RELEASED = HIGH;

private:
	uint8_t  pin_;
	uint16_t delay_;
	bool     state_;
	uint32_t ignoreUntil_;
	bool     hasChanged_;
	unsigned long pressedTime_ ;  // Track the time when button was pressed
	unsigned long longPressThreshold_ ; // Define the long press duration threshold (in milliseconds)
	bool longPressFlag_;
};

#endif