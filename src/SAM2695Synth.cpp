//
// Created by Administrator on 25-4-1.
//

#include "SAM2695Synth.h"

SAM2695Synth::SAM2695Synth()
    :_serial(nullptr)
    ,_pitch(60)
   ,_velocity(60)
{

}

SAM2695Synth& SAM2695Synth::getInstance()
{
    static SAM2695Synth instance;
    return instance;
}

void SAM2695Synth::begin(HardwareSerial* serial, int baud, uint8_t RX, uint8_t TX)
{
    _serial = serial;
    _serial->begin(baud, SERIAL_8N1, RX, TX);
}

void SAM2695Synth::setInstrument(uint8_t bank, uint8_t channel, uint8_t value)
{
    uint8_t CMD_CONTROL_CHANGE_1[] = {
        (uint8_t)(MIDI_CMD_CONTROL_CHANGE | (channel & 0x0f)), 0x00, bank};

    sendCMD(CMD_CONTROL_CHANGE_1, sizeof(CMD_CONTROL_CHANGE_1));

    uint8_t CMD_PROGRAM_CHANGE_2[] = {
        (uint8_t)(MIDI_CMD_PROGRAM_CHANGE | (channel & 0x0f)), value};
    sendCMD(CMD_PROGRAM_CHANGE_2, sizeof(CMD_PROGRAM_CHANGE_2));
}

void SAM2695Synth::setNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    uint8_t CMD_NOTE_ON[] = {(uint8_t)(MIDI_COMMAND_ON | (channel & 0x0f)),
                             pitch, velocity};
    sendCMD(CMD_NOTE_ON, sizeof(CMD_NOTE_ON));
}

void SAM2695Synth::setNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    uint8_t CMD_NOTE_OFF[] = {(uint8_t)(MIDI_COMMAND_OFF | (channel & 0x0f)),
                              pitch, 0x00};
    sendCMD(CMD_NOTE_OFF, sizeof(CMD_NOTE_OFF));
}

void SAM2695Synth::setAllNotesOff(uint8_t channel)
{
    uint8_t CMD_CONTROL_CHANGE[] = {
        (uint8_t)(MIDI_CMD_CONTROL_CHANGE | (channel & 0x0f)), 0x7b, 0x00};
    sendCMD(CMD_CONTROL_CHANGE, sizeof(CMD_CONTROL_CHANGE));
}

void SAM2695Synth::playOnChannel(uint8_t channel)
{
    uint8_t velocity = _velocity;
    uint8_t pitch = _pitch;
    uint8_t CMD_NOTE_ON[] = {(uint8_t)(MIDI_COMMAND_ON | (channel & 0x0f)),
                              pitch, velocity};
    sendCMD(CMD_NOTE_ON, sizeof(CMD_NOTE_ON));
}

void SAM2695Synth::setPitch(uint8_t pitch)
{
    _pitch = pitch;
}

void SAM2695Synth::setVelocity(uint8_t velocity)
{
    _velocity = velocity;
}

void SAM2695Synth::increasePitch()
{
    _pitch++;
    if(_pitch > BANK0_FX4atmosphere) _pitch = BANK0_FX4atmosphere;
}

void SAM2695Synth::decreasePitch()
{
    _pitch--;
    if(_pitch < BANK1_Clav) _pitch = BANK1_Clav;
}


void SAM2695Synth::increaseVelocity()
{
    _velocity += VELOCITY_STEP;
    if(_velocity > VELOCITY_MAX) _velocity = VELOCITY_MAX;
}

void SAM2695Synth::decreaseVelocity()
{
    _velocity -= VELOCITY_STEP;
    if(_velocity < VELOCITY_MIN) _velocity = VELOCITY_MIN;
}

void SAM2695Synth::drumPlay()
{

}

void SAM2695Synth::sendCMD(byte* cmd, int len)
{
    _serial->write(cmd, len);
}
