#include <Arduino.h>
#include "SAM2695Synth.h"

//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();

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
  synth.setNoteOn(9, 38, 127);
  delay(1000);
  synth.setNoteOn(9, 36, 127);
  delay(1000);
}
