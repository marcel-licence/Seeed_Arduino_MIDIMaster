/** 
 * This is a basic example code. The purpose of the code is to send MIDI signals via serial
 * from the XIAO series development board to make the SAM2695 chip produce sound.
 * You will need to define the serial port and its pins. This example uses the XIAO_ESP32S3.
 * For pin definitions, please refer to https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
 * 
 * Note: If you are using a different development board 
 * (e.g., XIAO_ESP32C3, documentation link https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/):
 *     1. Ensure the pin definitions are correct.
 *     2. Make sure the hardware serial port is defined correctly if you are using a hardware serial port.
 *     3. Check if the chip has relevant indicator lights; if not, external indicators may be needed unless you don't require indicator functionality.
 *     If using XIAO_ESP32S3, the following definitions should be used:
 *     #define SERIAL Serial1  // Hardware serial port for XIAO_ESP32C3
 *     #define BUTTON_A_PIN 2  // Button 1 on XIAO_ESP32C3
 *     #define BUTTON_B_PIN 3  // Button 2 on XIAO_ESP32C3
 *     #define BUTTON_C_PIN 4  // Button 3 on XIAO_ESP32C3
 *     #define BUTTON_D_PIN 5  // Button 4 on XIAO_ESP32C3
 *     For other models, refer to the corresponding documentation.
 * 
 * In this example, software serial is used for transmission.
 * To use a software serial port, you should first install the EspSoftwareSerial library.
 * 
 */


#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SAM2695Synth.h"

/*
Using software serial, so the hardware serial is commented out.
*/

// #define XIAO_TX 43
// #define XIAO_RX 44
// #define SERIAL Serial2

/*
Two ordinary GPIO pins are selected as the serial communication pins. 
Please refer to the relevant development board documentation for pin definitions.
*/
#define XIAO_TX_SW 6
#define XIAO_RX_SW 5
SoftwareSerial swSerial(XIAO_RX_SW, XIAO_TX_SW);//RX TX 


//create SAM2695Synth
SAM2695Synth synth = SAM2695Synth::getInstance();

void setup() 
{
    Serial.begin(9600);
    // 
    // synth.begin(&SERIAL, MIDI_SERIAL_BAUD_RATE, XIAO_RX, XIAO_TX);
    synth.begin(&swSerial,MIDI_SERIAL_BAUD_RATE);
    delay(1000);
}
void loop() 
{
    synth.setNoteOn(CHANNEL_0,NOTE_E4,VELOCITY_DEFAULT);
    delay(1000);
    synth.setNoteOff(CHANNEL_0,NOTE_E4);
    delay(1000);
}