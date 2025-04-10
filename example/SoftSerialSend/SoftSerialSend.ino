#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SAM2695Synth.h"
/*
use software serial , please download EspSoftwareSerial library
*/

//define serial  -- XIAO to SAM2695
#define XIAO_TX 44
#define XIAO_RX 43
//create software serial
SoftwareSerial swSerial(XIAO_TX, XIAO_RX);//TX RX 
//create SAM2695Synth
SAM2695Synth synth = SAM2695Synth::getInstance();

void setup() 
{
    Serial.begin(9600);
    //use software serial
    synth.begin(&swSerial,MIDI_SERIAL_BAUD_RATE);
    delay(3000);
    Serial.print("synth and serial ok!");
}
void loop() 
{
    synth.setNoteOn(CHANNEL_0,NOTE_E4,VELOCITY_DEFAULT);
    delay(1000);
    synth.setNoteOff(CHANNEL_0,NOTE_E4,VELOCITY_DEFAULT);
    delay(1000);
}