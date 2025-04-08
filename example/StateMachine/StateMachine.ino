#include <Arduino.h>
#include "ButtonState.h"
#include "SAM2695Synth.h"
#include "State.hpp"
#include "Button.h"

//LED toggle events corresponding to different modes
#define STATE_1_LED_TIME 2000
#define STATE_2_LED_TIME 500
#define STATE_3_LED_TIME 100

//Define the structure needed for the button
BtnState btnA = {HIGH, HIGH, 0, 0, false};
BtnState btnB = {HIGH, HIGH, 0, 0, false};
BtnState btnC = {HIGH, HIGH, 0, 0, false};
BtnState btnD = {HIGH, HIGH, 0, 0, false};

// Define an array of buttons and state machine events
ButtonFlags buttonFlags[] = {
    {shortPressFlag_A, longPressFlag_A, releaseFlag_A, EventType::APressed, EventType::ALongPressed},
    {shortPressFlag_B, longPressFlag_B, releaseFlag_B, EventType::BPressed, EventType::BLongPressed},
    {shortPressFlag_C, longPressFlag_C, releaseFlag_C, EventType::CPressed, EventType::CLongPressed},
    {shortPressFlag_D, longPressFlag_D, releaseFlag_D, EventType::DPressed, EventType::DLongPressed}
};

//Example of multi-track chord data definition
const musicData channel_1_chord =
{
    CHANNEL_9,
    {
        {NOTE_C2, true},
        {NOTE_FS2, true},
    },
    VELOCITY_DEFAULT ,
    BPM_DEFAULT + BPM_STEP,
};

const musicData channel_2_chord =
{
    CHANNEL_9,
    {
        {NOTE_FS2, true},
    },
    VELOCITY_DEFAULT ,
    BPM_DEFAULT - BPM_STEP,
};

const musicData channel_3_chord =
{
    CHANNEL_9,
    {
        {NOTE_D2, true},
        {NOTE_FS2, true},
    },
    VELOCITY_DEFAULT ,
    BPM_DEFAULT - BPM_STEP,
};
const musicData channel_4_chord =
{
    CHANNEL_9,
    {
        {NOTE_FS2, true},
    },
    VELOCITY_DEFAULT ,
    BPM_DEFAULT + BPM_STEP,
};

//get synth
SAM2695Synth synth = SAM2695Synth::getInstance();;
//create state machine and get state manager
StateMachine stateMachine;
StateManager* manager = StateManager::getInstance();


int beatCount = 0;                                  // Beat counter
int noteType = QUATER_NOTE;                         // Note type selection: 0 (quarter note), 1 (eighth note), 2 (sixteenth note)
int beatsPerBar = BEATS_BAR_DEFAULT;                // Beats per measure, can be 2, 3, or 4

unsigned long preMillisCh_1 = 0;                    // Record the time of the last MIDI signal sent on track 1
unsigned long preMillisCh_2 = 0;                    // Record the time of the last MIDI signal sent on track 2
unsigned long preMillisCh_3 = 0;                    // Record the time of the last MIDI signal sent on track 3
unsigned long preMillisCh_4 = 0;                    // Record the time of the last MIDI signal sent on track 4
unsigned long preMillisCh_drup = 0;                 // Record the time of the last MIDI signal sent on track drup
uint8_t drupCount = 0;                              // drup track count

//LED data
uint8_t  modeID = AuditionMode::ID;                 // state mode id 
int ledTime = STATE_1_LED_TIME;                     // LED toggle events TIME
unsigned long previousMillisLED = 0;                // Record the time of  the last LED toggle


void setup()
{
    //init usb serial port
    Serial.begin(USB_SERIAL_BAUD_RATE);
    //init LED
    pinMode(LED_BUILTIN, OUTPUT);
    initButtons();
    delay(3000);
    //init synth
    synth.begin();
    //regist three mode state
    manager->registerState(new AuditionMode());
    manager->registerState(new BpmMode());
    manager->registerState(new TrackMode());
    //regist error state
    ErrorState* errorState = new ErrorState();
    manager->registerState(errorState);
    //init state machine
    if(!(stateMachine.init(manager->getState(AuditionMode::ID), errorState)))
    {
        StateManager::releaseInstance();
        return ;
    }
    Serial.println("synth and state machine ready!");
}

void loop()
{
    Event* event = getNextEvent();
    if(event != nullptr)
    {
        stateMachine.handleEvent(event);
        delete event;
    }
    multiTrackPlay();
    ledShow();
}

Event* getNextEvent()
{
    // detectButtonEvents(BUTTON_A_PIN, btnA, act);
    detectButtonEvents(BUTTON_A_PIN, btnA, shortPressFlag_A, longPressFlag_A, releaseFlag_A);
    detectButtonEvents(BUTTON_B_PIN, btnB, shortPressFlag_B, longPressFlag_B, releaseFlag_B);
    detectButtonEvents(BUTTON_C_PIN, btnC, shortPressFlag_C, longPressFlag_C, releaseFlag_C);
    detectButtonEvents(BUTTON_D_PIN, btnD, shortPressFlag_D, longPressFlag_D, releaseFlag_D);

    // Check short press and long press flags
    for (const auto& flags : buttonFlags) {
        if (flags.shortPress) {
            flags.shortPress = false;
            return new Event(flags.shortPressType);
        }
        if (flags.longPress) {
            flags.longPress = false;
            return new Event(flags.longPressType);
        }
    }

    // Check release flag
    bool anyReleased = false;
    for (auto& flags : buttonFlags) {
        if (flags.release) {
            anyReleased = true;
            flags.release = false;
        }
    }

    if (anyReleased) {
        return new Event(EventType::BtnReleased);
    }

    return nullptr;
}

//LED show for diffent mode
void ledShow()
{
    modeID = stateMachine.getCurrentState()->getID();
    if(modeID == AuditionMode::ID)
    {
        ledTime = STATE_1_LED_TIME;
    }
    else if(modeID == BpmMode::ID)
    {
        ledTime = STATE_2_LED_TIME;
    }
    else if(modeID == TrackMode::ID)
    {
        ledTime = STATE_3_LED_TIME;
    }
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillisLED >= ledTime)
    {
        previousMillisLED = millis();
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}

//Multi-track chord play"
void multiTrackPlay()
{
    unsigned long currentMillis = millis();
    if(channel_1_on_off_flag)
    {
        if (currentMillis - preMillisCh_1 >= channel_1_chord.bpm)
        {
            preMillisCh_1 = currentMillis;
            synth.playChord(channel_1_chord);
        }
    }

    if(channel_2_on_off_flag)
    {
        if(currentMillis - preMillisCh_2 >= channel_2_chord.bpm)
        {
            preMillisCh_2 = currentMillis;
            synth.playChord(channel_2_chord);
        }
    }

    if(channel_3_on_off_flag)
    {
        if(currentMillis - preMillisCh_3 >= channel_3_chord.bpm)
        {
            preMillisCh_3 = currentMillis;
            synth.playChord(channel_3_chord);
        }
    }

    if(channel_4_on_off_flag)
    {
        if(currentMillis - preMillisCh_4 >= channel_4_chord.bpm)
        {
            preMillisCh_4 = currentMillis;
            synth.playChord(channel_4_chord);
        }
    }

    unsigned long interval = (BASIC_TIME / synth.getBpm()) / (noteType + 1);
    if (currentMillis - preMillisCh_drup >= interval)
    {
        preMillisCh_drup = currentMillis;
        if(drum_on_off_flag)
        {
            if(drupCount % 4 == 0)
            {
                synth.playChord(channel_1_chord);
            }
            else if(drupCount % 4 == 1)
            {
                synth.playChord(channel_2_chord);
            }
            else if(drupCount % 4 == 2)
            {
                synth.playChord(channel_3_chord);
            }
            else if(drupCount % 4 == 3)
            {
                synth.playChord(channel_4_chord);
            }
            drupCount++;
        }
    }
}