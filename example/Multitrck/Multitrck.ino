#include <Arduino.h>
#include "EmmaButton.hpp"
#include "SAM2695Synth.h"

/*
Timbre : 0~127
*/

SAM2695Synth synth = SAM2695Synth::getInstance();
EmmaButton button;

int bpm = BPM_DEFAULT;  //Default BPM
int noteType = QUATER_NOTE; // Note type selection: 0 (quarter note), 1 (eighth note), 2 (sixteenth note)
int beatCount = 0; //Recour beat count
unsigned long previousMillis = 0; // Record the time of the last sent MIDI signal.
bool buttonAFlag = false;
bool buttonBFlag = false;
bool buttonCFlag = false;
bool buttonDFlag = false;
void setup()
{
    //serial init to usb
    Serial.begin(115200);
    //synth init
    //the func default (Serial2,MIDI_SERIAL_BAUD_RATE,42,43);
    //synth.begin(); 
    synth.begin(&Serial2, MIDI_SERIAL_BAUD_RATE, 44, 43);
    Serial.println("SAM2695 Synth");
    delay(1000);
    //Set up the notes and sounds you want to play.
    synth.setPitch(42);
    synth.setVelocity(100);
    //you can set synth bpm (40 ~ 240)
    synth.setBpm(120);
}

void loop()
{
    // Calculate the interval between each beat (quarter note) in milliseconds
    // 60000 milliseconds equals 1 minute.
    unsigned long interval = (BASIC_TIME / synth.getBpm()) / (noteType + 1);

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        if(buttonAFlag)
        {
            synth.play(CHANNEL_0,38);
        }
        if(buttonBFlag)
        {
            synth.play(CHANNEL_0,48);
        }
        if(buttonCFlag)
        {
            synth.play(CHANNEL_0,52);
        }
        if(buttonDFlag)
        {
            synth.play(CHANNEL_0);
        }
    }
    //the channel on or off flag
    if(button.A.pressed() == BtnAct::Pressed)
    {
        buttonAFlag = !buttonAFlag;
    }
    if(button.B.pressed() == BtnAct::Pressed)
    {
        buttonBFlag = !buttonBFlag;
    }
    if(button.C.pressed() == BtnAct::Pressed)
    {
        buttonCFlag = !buttonCFlag;
    }
    if(button.D.pressed() == BtnAct::Pressed)
    {
        buttonDFlag = !buttonDFlag;
    }
}