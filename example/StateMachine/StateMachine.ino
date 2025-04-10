#include <Arduino.h>
#include "AuditionMode.h"
#include "SAM2695Synth.h"
#include "Button.h"
#include "BpmMode.h"
#include "TrackMode.h"
#include "ErrorState.h"

//LED toggle events corresponding to different modes
#define STATE_1_LED_TIME 2000
#define STATE_2_LED_TIME 500
#define STATE_3_LED_TIME 100
//define LED Pins 
#define LED_PIN LED_BUILTIN
//define buttons please look Button.h

//define serial  -- XIAO to SAM2695
#define XIAO_TX 44
#define XIAO_RX 43

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
    0,
    BPM_DEFAULT + BPM_STEP,
};

const musicData channel_2_chord =
{
    CHANNEL_9,
    {
        {NOTE_FS2, true},
    },
    VELOCITY_DEFAULT ,
    1,
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
    2,
    BPM_DEFAULT - BPM_STEP,
};

const musicData channel_4_chord =
{
    CHANNEL_9,
    {
        {NOTE_FS2, true},
    },
    VELOCITY_DEFAULT ,
    3,
    BPM_DEFAULT + BPM_STEP,
};
musicData trackArr[] = {channel_1_chord,channel_2_chord,channel_3_chord,channel_4_chord};

//create SAM2695Synth
SAM2695Synth synth = SAM2695Synth::getInstance();
//create state machine
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
Event eventArray[EVENT_ARR_SIZE];                   // create event array EventType : default EventType::None

//create soft serial
// SoftwareSerial swSerial(XIAO_TX, XIAO_RX);//TX RX 

void setup()
{
    //init usb serial port
    Serial.begin(USB_SERIAL_BAUD_RATE);
    //init LED
    pinMode(LED_PIN, OUTPUT);
    //init button
    initButtons();
    //init synth -- 
    synth.begin(&Serial2,MIDI_SERIAL_BAUD_RATE,XIAO_TX,XIAO_RX); //use hardware serial
    // synth.begin(&swSerial,MIDI_SERIAL_BAUD_RATE); // use software serial
    delay(3000);
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
        // set the event type is None
        event->setType(EventType::None);
        // Release the event's occupied status
        event->setInUse(false);
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

    for (const auto& flags : buttonFlags) {
        if (flags.shortPress) {
            flags.shortPress = false;
            // "Find the first unused event in the array
            for (int i = 0; i < EVENT_ARR_SIZE; i++) {
                if (!eventArray[i].isInUse()) {
                    // set the event type is shortPressType by button
                    eventArray[i].setType(flags.shortPressType);
                    // Mark as in use to prevent reuse
                    eventArray[i].setInUse(true);
                    return &eventArray[i];
                }
            }
        }
        if (flags.longPress) {
            flags.longPress = false;
            // "Find the first unused event in the array
            for (int i = 0; i < EVENT_ARR_SIZE; i++) {
                if (!eventArray[i].isInUse()) {
                    // set the event type is longPressType by button
                    eventArray[i].setType(flags.longPressType);
                    // Mark as in use to prevent reuse
                    eventArray[i].setInUse(true);
                    return &eventArray[i];
                }
            }
        }
    }

    bool anyReleased = false;
    for (auto& flags : buttonFlags) {
        if (flags.release) {
            anyReleased = true;
            flags.release = false;
        }
    }

    if (anyReleased) {
        // "Find the first unused event in the array
        for (int i = 0; i < EVENT_ARR_SIZE; i++) {
            if (!eventArray[i].isInUse()) {
                // set the event type is BtnReleased
                eventArray[i].setType(EventType::BtnReleased);
                // Mark as in use to prevent reuse
                eventArray[i].setInUse(true);
                return &eventArray[i];
            }
        }
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
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}

//Multi-track chord play"
void multiTrackPlay()
{
    unsigned long currentMillis = millis();
    if(channel_1_on_off_flag)
    {
        if (currentMillis - preMillisCh_1 >= channel_1_chord.delay)
        {
            preMillisCh_1 = currentMillis;
            synth.playChord(channel_1_chord);
        }
    }

    if(channel_2_on_off_flag)
    {
        if(currentMillis - preMillisCh_2 >= channel_2_chord.delay)
        {
            preMillisCh_2 = currentMillis;
            synth.playChord(channel_2_chord);
        }
    }

    if(channel_3_on_off_flag)
    {
        if(currentMillis - preMillisCh_3 >= channel_3_chord.delay)
        {
            preMillisCh_3 = currentMillis;
            synth.playChord(channel_3_chord);
        }
    }

    if(channel_4_on_off_flag)
    {
        if(currentMillis - preMillisCh_4 >= channel_4_chord.delay)
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
            if(trackArr[drupCount].index == drupCount )
            {
                synth.playChord(trackArr[drupCount]);
            }
            drupCount = (drupCount + 1) % (sizeof(trackArr)/sizeof(trackArr[0]));
        }
    }
}


