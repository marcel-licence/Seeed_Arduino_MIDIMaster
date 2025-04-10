/*
btn a : on or off music
btn b : Velocity++ step is VELOCITY_STEP
btn c : Velocity-- step is VELOCITY_STEP
*/

#include <Arduino.h>
#include "SAM2695Synth.h"
#include "Button.h"

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
BtnState btnC = {HIGH, HIGH, 0, 0, false};


bool music_on_off_flag = false;
int bpm = BPM_DEFAULT;                            // init BPM，BPM_DEFAULT = 120
int beatsPerBar = BEATS_BAR_DEFAULT;              // Beats per measure, which can be 2, 3, or 4
int noteType = QUATER_NOTE;                       // Note type selection: 0 (quarter note), 1 (eighth note), 2 (sixteenth note)
int beatCount = 0;                                // Beat counter
unsigned long previousMillis = 0;                 // Record the time of the last MIDI signal sent


void setup()
{
    //init usb serial port
    Serial.begin(USB_SERIAL_BAUD_RATE);
    //synth init
    synth.begin(&SERIAL, MIDI_SERIAL_BAUD_RATE, XIAO_RX, XIAO_TX);
    //buttons init,button defines please look Button.h
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