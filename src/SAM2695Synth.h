//
// Created by Administrator on 25-4-1.
//

#ifndef SAM2695SYNTH_H
#define SAM2695SYNTH_H

#include <Arduino.h>
#include "SAM2695_Def.h"

struct Note {
    uint8_t channel;
    uint8_t pitch;
    uint8_t velocity;
};

class SAM2695Synth {
public:
    static SAM2695Synth& getInstance();
    void begin(HardwareSerial *serial = &Serial2, int baud = MIDI_SERIAL_BAUD_RATE,
               uint8_t RX = 42, uint8_t TX = 43);

    void setInstrument(uint8_t bank, uint8_t channel, uint8_t value);
    void setNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
    void setNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
    void setAllNotesOff(uint8_t channel);
    void setPitch(uint8_t pitch);
    uint8_t getPitch() const;
    void setVelocity(uint8_t velocity);
    void increasePitch();
    void decreasePitch();
    void increaseVelocity();
    void decreaseVelocity();
    void increaseBpm();
    void decreaseBpm();
    void setBpm(uint8_t bpm);
    uint8_t getBpm() const;


private:
    SAM2695Synth();
    void sendCMD(byte* cmd,int len);

private:
    HardwareSerial*   _serial;
    uint8_t           _pitch;
    uint8_t           _velocity;
    uint8_t           _bpm;
};



#endif //SAM2695SYNTH_H
