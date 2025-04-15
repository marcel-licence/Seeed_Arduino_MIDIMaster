/** 
 * This is a basic example code. The purpose of the code is to send MIDI signals via serial
 * from the XIAO series development board to make the SAM2695 chip produce sound.
 * You will need to define the serial port and its pins. This example uses the XIAO_ESP32S3.
 * For pin definitions, please refer to https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
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
 
 #ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(2, 3); // RX, TX
    #define COM_SERIAL SSerial
    #define SHOW_SERIAL Serial
    SAM2695Synth<SoftwareSerial> synth = SAM2695Synth<SoftwareSerial>::getInstance();
    #endif

#if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_RP2350) ||  defined(ARDUINO_XIAO_RA4M1) 
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(D7, D6); // RX, TX
    #define COM_SERIAL SSerial
    #define SHOW_SERIAL Serial
    SAM2695Synth<SoftwareSerial> synth = SAM2695Synth<SoftwareSerial>::getInstance();
#endif

#if  defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6) || defined(CONFIG_IDF_TARGET_ESP32S3)
    #define COM_SERIAL Serial0
    #define SHOW_SERIAL Serial
    SAM2695Synth<HardwareSerial> synth = SAM2695Synth<HardwareSerial>::getInstance();
#endif

#ifdef SEEED_XIAO_M0
    #define COM_SERIAL Serial1
    #define SHOW_SERIAL Serial
    SAM2695Synth<Uart> synth = SAM2695Synth<Uart>::getInstance();
#elif defined(ARDUINO_SAMD_VARIANT_COMPLIANCE)
    #define COM_SERIAL Serial1
    #define SHOW_SERIAL SerialUSB
    SAM2695Synth<Uart> synth = SAM2695Synth<Uart>::getInstance();
#endif

#if defined(NRF52840_XXAA)
    #ifdef USE_TINYUSB
    #include <Adafruit_TinyUSB.h>
    #endif
    #define COM_SERIAL Serial1
    #define SHOW_SERIAL Serial

    SAM2695Synth<Uart> synth = SAM2695Synth<Uart>::getInstance();
#endif

#if defined(CONFIG_IDF_TARGET_ESP32S3)
    #define BUTTON_A_PIN 1
    #define BUTTON_B_PIN 2
    #define BUTTON_C_PIN 3 
    #define BUTTON_D_PIN 4 
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
    #define BUTTON_A_PIN 2
    #define BUTTON_B_PIN 3
    #define BUTTON_C_PIN 4 
    #define BUTTON_D_PIN 5 
#elif defined(CONFIG_IDF_TARGET_ESP32C6)
    #define BUTTON_A_PIN 0
    #define BUTTON_B_PIN 1
    #define BUTTON_C_PIN 2 
    #define BUTTON_D_PIN 21
#else //You need to refer to the corresponding manual...
    #define BUTTON_A_PIN 0 //You need to refer to the corresponding manual...
    #define BUTTON_B_PIN 1 //You need to refer to the corresponding manual...
    #define BUTTON_C_PIN 2 //You need to refer to the corresponding manual...
    #define BUTTON_D_PIN 3 //You need to refer to the corresponding manual...
#endif

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
    SHOW_SERIAL.begin(USB_SERIAL_BAUD_RATE);
    // Synth initialization. Since a hardware serial port is used here, the software serial port is commented out.
    synth.begin(COM_SERIAL, MIDI_SERIAL_BAUD_RATE);
    synth.setInstrument(0,CHANNEL_0,unit_synth_instrument_t::GrandPiano_1);
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


