//
// Created by Administrator on 25-4-7.
//

#ifndef SOFTWARESERIAL_H
#define SOFTWARESERIAL_H


#include <Arduino.h>

class SoftwareSerial {
public:
    SoftwareSerial(byte rx, byte tx, unsigned long baudRate);
    void begin();
    void sendByte(byte data);
    byte receiveByte();
private:
    unsigned long bitDelay;
    void startBit();
    void stopBit();
    void sendBit(bool bit);
    bool receiveBit();
private:
    byte rxPin;
    byte txPin;
};


#endif //SOFTWARESERIALCLASS_H
