/*
btn a : on or off chord_twoNotes
btn b : on or off chord_fourNotes
*/

#include <Arduino.h>
#include "SAM2695Synth.h"
#include "Button.h"

//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();
//Define the structure needed for the button
BtnState btnA = {HIGH, HIGH, 0, 0, false};
BtnState btnB = {HIGH, HIGH, 0, 0, false};
BtnState btnC = {HIGH, HIGH, 0, 0, false};

//Example of multi-track chord data definition
// This structure controls playback on channel 9, with only two notes being played. 
// The sound is set to the default sound, and the duration can be adjusted as needed.
const musicData chordTwoNotes =
{
    CHANNEL_9,                          //play on channel 9
    {
        {NOTE_C2, true},                //NOTE_C2 on
        {NOTE_FS2, true},               //NOTE_FS2 on
        {NOTE_E6, false},               //NOTE_E6 off
        {NOTE_CS5, false},              //NOTE_CS5 off
    },
    VELOCITY_DEFAULT ,                  //default velocity(64)
    BPM_DEFAULT*8,                      //interval time
};

const musicData chordFourNotes =
{
    CHANNEL_0,                          //play on channel 0
    {
        {NOTE_F2, true},                //NOTE_F2 on
        {NOTE_B2, true},                //NOTE_B2 on
        {NOTE_E6, true},                //NOTE_E6 on
        {NOTE_CS5, true},               //NOTE_CS5 off
    },
    VELOCITY_DEFAULT ,                  //default velocity(64)
    BPM_DEFAULT*4,                      //interval time
};

bool chordTwoNotesFalg = false;         //on or off chord_twoNotes
bool chordFourNotesFalg = false;        //on or off chord_fourNotes
unsigned long preMillisTwoNotes = 0;    // Record the time of the last MIDI signal sent on chord_twoNotes
unsigned long preMillisFourNotes= 0;    // Record the time of the last MIDI signal sent on const musicData chord_fourNotes =


void setup()
{
    //init usb serial port
    Serial.begin(USB_SERIAL_BAUD_RATE);
    delay(3000);
    //synth init
    synth.begin();
    //buttons init
    initButtons();
}

void loop()
{
    detectButtonEvents(BUTTON_A_PIN, btnA, shortPressFlag_A, longPressFlag_A, releaseFlag_A);
    detectButtonEvents(BUTTON_B_PIN, btnB, shortPressFlag_B, longPressFlag_B, releaseFlag_B);
    if(shortPressFlag_A)
    {
        shortPressFlag_A = false;
        chordTwoNotesFalg = !chordTwoNotesFalg;
    }
    if(shortPressFlag_B)
    {
        shortPressFlag_B = false;
        chordFourNotesFalg = !chordFourNotesFalg;
    }
    play();
}

void play()
{
    unsigned long currentMillis = millis();
    if(chordTwoNotesFalg)
    {
        if (currentMillis - preMillisTwoNotes >= chordTwoNotes.bpm)
        {
            preMillisTwoNotes = currentMillis;
            synth.playChord(chordTwoNotes);
        }
    }

    if(chordFourNotesFalg)
    {
        if (currentMillis - preMillisFourNotes >= chordFourNotes.bpm)
        {
            preMillisFourNotes = currentMillis;
            synth.playChord(chordFourNotes);
        }
    }
}