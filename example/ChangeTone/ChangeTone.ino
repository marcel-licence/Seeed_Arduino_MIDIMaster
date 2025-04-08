#include <Arduino.h>
#include "SAM2695Synth.h"

//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();
//Define the structure needed for the button
BtnState btnA = {HIGH, HIGH, 0, 0, false};

void setup
{
    //init usb serial port
    Serial.begin(USB_SERIAL_BAUD_RATE);
    delay(3000);
    //synth init
    synth.begin();
    //buttons init
    initButtons();
}

void loop
{
    detectButtonEvents(BUTTON_A_PIN, btnA, shortPressFlag_A, longPressFlag_A, releaseFlag_A);
    if(shortPressFlag_A){
        shortPressFlag_A = false;
        static uint8_t instrument = unit_synth_instrument_t::StringEnsemble2;
        instrument++;
        Serial.println("Instrument: " + String(instrument));
        synth.setInstrument(0,CHANNEL_0,instrument);
        synth.setPitch(instrument);
        synth.setNoteOn(CHANNEL_0,synth.getPitch(),VELOCITY_MAX);		
    }
}