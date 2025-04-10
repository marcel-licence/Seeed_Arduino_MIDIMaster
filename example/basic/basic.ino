/** 
 * 
*/

#include <Arduino.h>
#include "SAM2695Synth.h"

//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();
//define serial  -- XIAO to SAM2695
#define XIAO_TX 43
#define XIAO_RX 44
//define serial
#define SERIAL Serial2

void setup()
{
  //serial init to usb
  Serial.begin(115200);
  //synth init
  synth.begin(&SERIAL, MIDI_SERIAL_BAUD_RATE, XIAO_RX, XIAO_TX);
  delay(1000);
}

void loop()
{
  synth.setNoteOn(CHANNEL_0,NOTE_E4,VELOCITY_DEFAULT);
  delay(1000);
  synth.setNoteOff(CHANNEL_0,NOTE_E4,VELOCITY_DEFAULT);
  delay(1000);
}
