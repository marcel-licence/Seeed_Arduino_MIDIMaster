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
 * In this example, you can adjust the volume.
 * Button A controls the music on/off.
 * Button B increases the volume.
 * Button C decreases the volume.
 * The step size for increasing and decreasing can be found in the SAM2695_Def.h file and can be modified.
 */

 #include <Arduino.h>
 #include "SAM2695Synth.h"
 #include "Button.h"
 
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
BtnState btnC = {HIGH, HIGH, 0, 0, false};

bool music_on_off_flag = false;
int bpm = BPM_DEFAULT;                            // init BPM，BPM_DEFAULT = 120
int beatsPerBar = BEATS_BAR_DEFAULT;              // Beats per measure, which can be 2, 3, or 4
int noteType = QUATER_NOTE;                       // Note type selection: 0 (quarter note), 1 (eighth note), 2 (sixteenth note)
int beatCount = 0;                                // Beat counter
unsigned long previousMillis = 0;                 // Record the time of the last MIDI signal sent


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
    initButtons(BUTTON_C_PIN);

    delay(1000);
}

void loop()
{
    detectButtonEvents(BUTTON_A_PIN, btnA, shortPressFlag_A, longPressFlag_A, releaseFlag_A);
    detectButtonEvents(BUTTON_B_PIN, btnB, shortPressFlag_B, longPressFlag_B, releaseFlag_B);
    detectButtonEvents(BUTTON_C_PIN, btnC, shortPressFlag_C, longPressFlag_C, releaseFlag_C);
    if(shortPressFlag_A){
        shortPressFlag_A = false;
        music_on_off_flag = !music_on_off_flag;
    }
    if(shortPressFlag_B){
        shortPressFlag_B = false;
        synth.increaseVelocity();
    }
    if(shortPressFlag_C){
        shortPressFlag_C = false;
        synth.decreaseVelocity();
    }
    play();
}

void play()
{
    // Calculate the interval between each beat (quarter note) in milliseconds
    // 60,000 milliseconds equals 1 minute
    // Adjust interval based on note type
    unsigned long interval = (BASIC_TIME / synth.getBpm()) / (noteType + 1); 

    if(music_on_off_flag)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;
            if (beatCount == 0)
                synth.setNoteOn(CHANNEL_9, NOTE_D2, VELOCITY_MAX);
            else
                synth.setNoteOn(CHANNEL_9, NOTE_C2, VELOCITY_MAX);
    
            // Increment beat count
            beatCount++;
    
            // Every four beats, indicating the end of a measure
            if (beatCount >= beatsPerBar)
            {
                // Reset count
                beatCount = 0;
            }
        }
    }
}