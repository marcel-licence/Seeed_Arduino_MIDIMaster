#include <Arduino.h>
#include "EmmaButton.hpp"
#include "SAM2695Synth.h"

/*
BPM : 40 ~ 240
*/

SAM2695Synth synth = SAM2695Synth::getInstance();
EmmaButton button;

int bpm = BPM_DEFAULT;  //Default BPM
int beatsPerBar = BEATS_BAR_DEFAULT;// The number of beats per measure (2~4)
int noteType = QUATER_NOTE; // Note type selection: 0 (quarter note), 1 (eighth note), 2 (sixteenth note)
int beatCount = 0; //Recour beat count
unsigned long previousMillis = 0; // Record the time of the last sent MIDI signal.

void setup()
{
    //serial init to usb
    Serial.begin(115200);
    //synth init
    //synth.begin(); the func default (Serial2,MIDI_SERIAL_BAUD_RATE,42,43);
    synth.begin(&Serial2, MIDI_SERIAL_BAUD_RATE, 44, 43);
    Serial.println("SAM2695 Synth");
    delay(1000);
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
        if (beatCount == 0)
            synth.setNoteOn(9, 38, 40);
        else
            synth.setNoteOn(9, 36, 40);

        beatCount++; // add beatCount

        // Every four beats indicates the end of a measure
        if (beatCount >= beatsPerBar)
        {
            beatCount = 0; // reset beatCount
        }
    }

    //change bpm 
    //button A pressed
    if(button.A.pressed()==BtnAct::Pressed)
    {
        Serial.println(" Button A  Pressed");
        synth.increaseBpm();
    }
    //button B pressed
    if(button.B.pressed()==BtnAct::Pressed)
    {
        Serial.println(" Button B  Pressed");
        synth.decreaseBpm();
    }
}





