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
 * This example demonstrates how to play a piece of music composed of multiple chords.
 * You need to first define the structure required for the chord, which includes:
 *      Channel
 *      Note combination (the chord consists of multiple notes), with the second note being a flag that controls whether the note plays
 *      Volume
 *      Index 
 *      Tempo
 * Then define an array of music composed of multiple chords, which will use the index from the above structure.
 * Using a technical approach, when the index value equals the count value, the chord will be played.
 * The example provides two music samples, which can be viewed in the music.h file, and custom music can be defined for playback.
 * Button A controls the playback of the first piece of music.
 * Button B controls the playback of the second piece of music.
 */

 #include <Arduino.h>
 #include "SAM2695Synth.h"
 #include "Button.h"
 #include "music.h"
 
 /*
 Define the hardware serial port and its pins. If you need to use a hardware serial port
 please refer to the documentation of the corresponding development board for specific hardware pin definitions.
 */ 
 #define XIAO_TX 43
 #define XIAO_RX 44
 #define SERIAL Serial2
 /*
 Define the software serial port and its pins. If you need to use a software serial port
 this example uses a hardware serial port, so the software serial port is commented out.
 To use a software serial port, you should first install the EspSoftwareSerial library.
 */ 
 // #include <SoftwareSerial.h>
 // #define XIAO_TX_SW 7
 // #define XIAO_RX_SW 8
 // SoftwareSerial swSerial(XIAO_RX, XIAO_TX);//RX TX 
 
 // Define the buttons. The pins here correspond to the XIAO_ESP32S3. 
 // For other platforms, you need to refer to the corresponding documentation.
 #define BUTTON_A_PIN 1  // Button 1 on XIAO_ESP32S3
 #define BUTTON_B_PIN 2  // Button 2 on XIAO_ESP32S3
 #define BUTTON_C_PIN 3  // Button 3 on XIAO_ESP32S3
 #define BUTTON_D_PIN 4  // Button 4 on XIAO_ESP32S3

//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();
//Define the structure needed for the button
BtnState btnA = {HIGH, HIGH, 0, 0, false};
BtnState btnB = {HIGH, HIGH, 0, 0, false};

bool track_1_flag = false;                          // on or off track 1 music
bool track_2_flag = false;                          // on or off track 2 music
unsigned long preMillisCh_1 = 0;                    // Record the time of the last MIDI signal sent on track 1
unsigned long preMillisCh_2 = 0;                    // Record the time of the last MIDI signal sent on track 2
uint8_t countBytrack1 = 0;                          // Index count of Music 1
uint8_t countBytrack2 = 0;                          // Index count of Music 2                        


void setup()
{
    //serial init to usb
    Serial.begin(115200);
    // Synth initialization. Since a hardware serial port is used here, the software serial port is commented out.
    synth.begin(&SERIAL, MIDI_SERIAL_BAUD_RATE, XIAO_RX, XIAO_TX);
    // synth.begin(&swSerial,MIDI_SERIAL_BAUD_RATE);
    // Initialize the buttons you are using.
    initButtons(BUTTON_A_PIN);
    initButtons(BUTTON_B_PIN);
    delay(1000);
}

void loop() 
{
    detectButtonEvents(BUTTON_A_PIN, btnA, shortPressFlag_A, longPressFlag_A, releaseFlag_A);
    detectButtonEvents(BUTTON_B_PIN, btnB, shortPressFlag_B, longPressFlag_B, releaseFlag_B);
    if(shortPressFlag_A)
    {
        shortPressFlag_A = false;
        track_1_flag = !track_1_flag;
    }
    if(shortPressFlag_B)
    {
        shortPressFlag_B = false;
        track_2_flag = !track_2_flag;
    }
    play();
}

void play()
{
    unsigned long currentMillis = millis();
    if (currentMillis - preMillisCh_1 >= track1[countBytrack1].delay)
    {
        preMillisCh_1 = currentMillis;
        if(track_1_flag)
        {
            if(track1[countBytrack1].index == countBytrack1)
            {
                synth.playChord(track1[countBytrack1]);
                countBytrack1 = (countBytrack1 + 1) % (sizeof(track1) / sizeof(track1[0]));
            }
        }
    }

    if (currentMillis - preMillisCh_2 >= track2[countBytrack2].delay)
    {
        preMillisCh_2 = currentMillis;
        if(track_2_flag)
        {
            if(track2[countBytrack2].index == countBytrack2)
            {
                synth.playChord(track2[countBytrack2]);
                countBytrack2 = (countBytrack2 + 1) % (sizeof(track2) / sizeof(track2[0]));
            }
        }
    }
}


