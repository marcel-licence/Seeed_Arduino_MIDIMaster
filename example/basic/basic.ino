#include <Arduino.h>
#include "EmmaButton.hpp"
#include "SAM2695_Driver.h"
#include "SAM2695_Def.h"

//creat object of EmmaButton and SAM2695_Driver
EmmaButton button;
SAM2695_Driver seq = SAM2695_Driver();

//set global variable

//midi callback function
void midi(byte channel, byte command, byte arg1, byte arg2)
{
    if(command < 128) {
        command <<= 4;
        command |= channel;
    }
    byte CMD_NOTE_ON[] ={command, arg1, arg2};
    // seq._serial->write(command);
    // seq._serial->write(arg1);
    // seq._serial->write(arg2);
    seq.sendCMD(CMD_NOTE_ON, sizeof(CMD_NOTE_ON));
}

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
    }
     
    if (button.A.released() == BtnAct::released)
    {
        // @param4 volume 0-127
        midi(CHANNEL_0, MIDI_COMMAND_OFF, BANK0_BlownBottle, 0);
    }
    seq.run();
}

