/**
 * Example of how to play chord progressions, supporting simultaneous playback of multiple tracks.
 * Button A controls the progression on track 0, while button B controls the progression on track 9.
 * Before playing, you need to define your chord progression, including the channel to play on, individual or multiple notes, sounds, index, and tempo.
 * Each structure should represent a chord, consisting of an array that forms a segment of the chord progression.
 */


#include <Arduino.h>
#include "SAM2695Synth.h"
#include "Button.h"
#include "music.h"

//define serial  -- XIAO to SAM2695
#define XIAO_TX 43
#define XIAO_RX 44
//define serial
#define SERIAL Serial2

//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();
//Define the structure needed for the button
BtnState btnA = {HIGH, HIGH, 0, 0, false};
BtnState btnB = {HIGH, HIGH, 0, 0, false};

bool track_1_flag = false;                          // on or off track 1 music
bool track_2_flag = false;                          // on or off track 2 music
unsigned long preMillisCh_1 = 0;                    // Record the time of the last MIDI signal sent on track 1
unsigned long preMillisCh_2 = 0;                    // Record the time of the last MIDI signal sent on track 2
uint8_t countBytrack1 = 0;
uint8_t countBytrack2 = 0;



void setup()
{
    //init usb serial port
    Serial.begin(USB_SERIAL_BAUD_RATE);
    //synth init
    synth.begin(&SERIAL, MIDI_SERIAL_BAUD_RATE, XIAO_RX, XIAO_TX);
    //buttons init,button defines please look Button.h
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


