#include <Arduino.h>
#include "EmmaButton.hpp"
#include "SAM2695Synth.h"

/*
Timbre : 0~127
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
    //the func default (Serial2,MIDI_SERIAL_BAUD_RATE,42,43);
    //synth.begin(); 
    synth.begin(&Serial2, MIDI_SERIAL_BAUD_RATE, 44, 43);
    Serial.println("SAM2695 Synth");
    delay(1000);
    //Set up the notes and sounds you want to play.
    synth.setPitch(42);
    synth.setVelocity(80);
    //you can set synth bpm (40 ~ 240)
    synth.setBpm(80);
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
        srand(static_cast<unsigned int>(time(0)));
        int randomNumber = rand() % 128;
        synth.play(CHANNEL_0,randomNumber);
        
    }
    //change timbre
    if(button.A.pressed() == BtnAct::Pressed)
    {
        //Generate a random timbre (instrument) sequence number
        srand(static_cast<unsigned int>(time(0)));
        int randomNumber = rand() % 128;
        synth.setInstrument(0,0,randomNumber);
    }
}