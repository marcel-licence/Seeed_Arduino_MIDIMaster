#include "SoftwareSerial.h"

/*
Send Mode
param1 =  RX pin 
param2 =  TX pin 
param3 = baudRate
*/
SoftwareSerial mySerial(42, 43, 31250);
void setup() {
    Serial.begin(9600);
    mySerial.begin();
}
void loop() {
    byte CMD_NOTE_ON[] = {(uint8_t)(0x90 | (0 & 0x0f)),64, 50};
    for (int i = 0; i < sizeof(CMD_NOTE_ON); i++) {
        mySerial.sendByte(CMD_NOTE_ON[i]);
    }
    delay(1000);
}