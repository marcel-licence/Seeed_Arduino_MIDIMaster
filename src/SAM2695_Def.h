#pragma once

#ifndef _SAM2695_DEF_H
#define _SAM2695_DEF_H

/*------------------------------------------- Defines begin -------------------------------------------*/
#define DEFAULT_BPM 120
#define DEFAULT_STEPS 16
#define DEFAULT_MEMORY 512
#define MIN_TEMPO 40
#define MAX_TEMPO 240
#define MAX_STEPS 256
/*------------------------------------------- Defines end -------------------------------------------*/

/*------------------------------------------- Structs begin -------------------------------------------*/
typedef struct
{
    byte channel;
    byte pitch;
    byte velocity;
    byte step;
} StepNote;

//! add new struct for test
struct StepNote2
{
    byte channel;
    byte pitch;
    byte velocity;
    byte bpm;
};

// default values for sequence array members
const StepNote DEFAULT_NOTE = {0x0, 0x0, 0x0, 0x0};
/*------------------------------------------- Structs end -------------------------------------------*/

/*------------------------------------------- callback function begin -------------------------------------------*/
using MIDIcallback = void (*)(byte channel, byte command, byte arg1, byte arg2);
using StepCallback = void (*)(int current, int last);
//! add new callback function type for test
using MIDIcallBack2 = void(*)(struct StepNote2& note);
/**------------------------------------------- callback function end -------------------------------------------*/

#endif