//
// Created by Administrator on 25-4-1.
//

#ifndef SAM2695SYNTH_H
#define SAM2695SYNTH_H

#include <Arduino.h>
#include "SAM2695_Def.h"
#include <SoftwareSerial.h>

class SAM2695Synth {
public:
    ~SAM2695Synth();
    static SAM2695Synth& getInstance();
    void begin(HardwareSerial *serial , int baud , uint8_t RX , uint8_t TX );
    void begin(SoftwareSerial *serial , int baud );

    void setInstrument(uint8_t bank, uint8_t channel, uint8_t value);
    void setNoteOn(uint8_t channel, uint8_t pitch = -1, uint8_t velocity = -1);
    void setNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
    void setAllNotesOff(uint8_t channel);
    void playChord(const musicData& chord);
    void setPitch(uint8_t pitch);
    uint8_t getPitch() const;
    void setVolume(uint8_t channel, uint8_t level);
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
    SoftwareSerial*   _softSerial; 
    uint8_t           _pitch;
    uint8_t           _velocity;
    uint8_t           _bpm;
    bool              _useHardwareSerial;
};



#endif //SAM2695SYNTH_H
