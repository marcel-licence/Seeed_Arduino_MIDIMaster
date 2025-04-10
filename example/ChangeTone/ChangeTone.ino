/** 
 * 
*/

#include <Arduino.h>
#include "SAM2695Synth.h"
#include "Button.h"

//define serial  -- XIAO to SAM2695
#define XIAO_TX 43
#define XIAO_RX 44
//define serial
#define SERIAL Serial2


//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();
//Define the structure needed for the button
BtnState btnA = {HIGH, HIGH, 0, 0, false};

void setup()
{
    //init usb serial port
    Serial.begin(USB_SERIAL_BAUD_RATE);
    //synth init
    synth.begin(&SERIAL, MIDI_SERIAL_BAUD_RATE, XIAO_RX, XIAO_TX);
    //buttons init,button defines please look Button.h
    initButtons(BUTTON_A_PIN);
    delay(1000);
}

void loop()
{
    detectButtonEvents(BUTTON_A_PIN, btnA, shortPressFlag_A, longPressFlag_A, releaseFlag_A);
    if(shortPressFlag_A)
    {
        shortPressFlag_A = false;
        static uint8_t instrument = unit_synth_instrument_t::StringEnsemble2;
        instrument++;
        Serial.println("Instrument: " + String(instrument));
        for(int i = CHANNEL_0;i<=CHANNEL_15;i++)
        {
            synth.setInstrument(0,i,instrument);
        }
        synth.setPitch(instrument);	
        synth.setNoteOn(CHANNEL_0,synth.getPitch(),VELOCITY_MAX);
        
    }

    if(releaseFlag_A)
    {
        releaseFlag_A = false;
        synth.setNoteOff(CHANNEL_0,synth.getPitch(),VELOCITY_MAX);	
    }

}