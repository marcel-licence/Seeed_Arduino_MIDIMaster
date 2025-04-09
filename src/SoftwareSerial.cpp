#include "SoftwareSerial.h"

SoftwareSerial::SoftwareSerial(byte rx, byte tx, unsigned long baudRate) {
    rxPin = rx;
    txPin = tx;
    bitDelay = 1000000 / baudRate;
}

void SoftwareSerial::begin() {
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    digitalWrite(txPin, HIGH);
}

void SoftwareSerial::startBit() {
    digitalWrite(txPin, LOW);
    delayMicroseconds(bitDelay);
}

void SoftwareSerial::stopBit() {
    digitalWrite(txPin, HIGH);
    delayMicroseconds(bitDelay);
}

void SoftwareSerial::sendBit(bool bit) {
    digitalWrite(txPin, bit);
    delayMicroseconds(bitDelay);
}

bool SoftwareSerial::receiveBit() {
    delayMicroseconds(bitDelay / 2);
    bool bit = digitalRead(rxPin);
    delayMicroseconds(bitDelay / 2);
    return bit;
}

void SoftwareSerial::sendByte(byte data) {
    startBit();
    for (int i = 0; i < 8; i++) {
        sendBit(bitRead(data, i));
    }
    stopBit();
}

byte SoftwareSerial::receiveByte() {
    while (digitalRead(rxPin) == HIGH);
    delayMicroseconds(bitDelay / 2);
    byte data = 0;
    for (int i = 0; i < 8; i++) {
        bitWrite(data, i, receiveBit());
    }
    delayMicroseconds(bitDelay);
    return data;
}