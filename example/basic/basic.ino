#include <Arduino.h>
#include "EmmaButton.hpp"
#include "SAM2695_Driver.h"
#include "SAM2695_Def.h"

//creat object of EmmaButton and SAM2695_Driver
EmmaButton button;
SAM2695_Driver seq = SAM2695_Driver();
//set global variable


void setup()
{
    Serial.begin(USB_SERIAL_BAUD_RATE);
    seq.begin();
    seq.setMidiHandler(midi);
    delay(3000);
    Serial.println("EmmaButton and SAM2695_Driver are ready!");
}

void loop()
{
    if (button.A.pressed() == BtnAct::pressed)
    {
        // @param4 volume 0-127
        midi(CHANNEL_0, MIDI_COMMAND_ON, BANK0_BlownBottle, 127);
        seq.setNote(CHANNEL_0, BANK0_BlownBottle, 127);
        Serial.println("Button pressed!");
    }
     
    if (button.A.released() == BtnAct::released)
    {
        // @param4 volume 0-127
        midi(CHANNEL_0, MIDI_COMMAND_OFF, BANK0_BlownBottle, 0);
        seq.setNote(CHANNEL_0, BANK0_BlownBottle, 0);
        Serial.println("Button released!");
    }
    seq.run();
}

void step(int current, int last) {
  // blink on even steps
  if(current % 2 == 0)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
}

void midi(byte channel, byte command, byte arg1, byte arg2)
{
  
  if(command < 128) {
        command <<= 4;
        command |= channel;
  }
  byte CMD_NOTE_ON[] ={command, arg1, arg2};
  seq.sendCMD(CMD_NOTE_ON, sizeof(CMD_NOTE_ON));
  Serial.println("MIDI message send ok!");
}

