//
// Created by Administrator on 25-4-1.
//

#include "SAM2695Synth.h"


SAM2695Synth::SAM2695Synth()
:_serial(nullptr)
,_pitch(60)
,_velocity(90)
,_bpm(BPM_DEFAULT)
{

}

// Get the singleton, ensuring there is only one object.
SAM2695Synth& SAM2695Synth::getInstance()
{
    static SAM2695Synth instance;
    return instance;
}

// Initializes the SAM2695Synth object with a specified serial interface and baud rate
// This version is called when the 'begin' function is invoked with custom serial, baud, RX, and TX settings.
// Parameters:
//   serial - Pointer to the HardwareSerial object for communication (e.g., Serial1, Serial2) 
//            //default Serial2
//   baud - Baud rate for serial communication (e.g., 9600, 115200) 
//           //default 115200
//   RX - The pin number used for receiving data (RX)
//            //default 42
//   TX - The pin number used for transmitting data (TX)
//            //default 43
void SAM2695Synth::begin(HardwareSerial* serial, int baud, uint8_t RX, uint8_t TX)
{
    _serial = serial;
    _serial->begin(baud, SERIAL_8N1, RX, TX);
}

// Sets the instrument for a specific MIDI bank and channel by sending appropriate MIDI control and program change messages
// This version is called when the 'setInstrument' function is invoked with user-defined bank, channel, and value parameters.
// Parameters:
//   bank - The bank number (0-127) to select the instrument from.
//   channel - The MIDI channel (0-15) to which the instrument should be assigned.
//   value - The program change value (instrument number) to be applied on the specified channel and bank.
void SAM2695Synth::setInstrument(uint8_t bank, uint8_t channel, uint8_t value)
{
    uint8_t CMD_CONTROL_CHANGE_1[] = {
        (uint8_t)(MIDI_CMD_CONTROL_CHANGE | (channel & 0x0f)), 0x00, bank};

    sendCMD(CMD_CONTROL_CHANGE_1, sizeof(CMD_CONTROL_CHANGE_1));

    uint8_t CMD_PROGRAM_CHANGE_2[] = {
        (uint8_t)(MIDI_CMD_PROGRAM_CHANGE | (channel & 0x0f)), value};
    sendCMD(CMD_PROGRAM_CHANGE_2, sizeof(CMD_PROGRAM_CHANGE_2));
    setPitch(value);
    setNoteOn(channel,value,_velocity);
}

// Sends a MIDI "Note On" message to trigger a note on a specific MIDI channel
// This will play the note with the specified pitch and velocity on the given channel.
// Parameters:
//   channel - The MIDI channel (0-15) on which to send the "Note On" message.
//   pitch - The MIDI pitch value (0-127) representing the note to be played (e.g., 60 for Middle C).
//   velocity - The velocity (0-127) indicating how hard the note is struck (higher values for louder sounds).
void SAM2695Synth::setNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    uint8_t CMD_NOTE_ON[] = {(uint8_t)(MIDI_COMMAND_ON | (channel & 0x0f)),
                             pitch, velocity};
    sendCMD(CMD_NOTE_ON, sizeof(CMD_NOTE_ON));
    return ;
}

// Sends a MIDI "Note Off" message to stop a note on a specific MIDI channel
// This will stop the note with the specified pitch on the given channel. 
// The velocity is typically set to 0 for "Note Off" messages.
// Parameters:
//   channel - The MIDI channel (0-15) on which to send the "Note Off" message.
//   pitch - The MIDI pitch value (0-127) representing the note to be stopped (e.g., 60 for Middle C).
//   velocity - The velocity (0-127) which is usually ignored for "Note Off" messages but can be set to any value. In this case, it's always set to 0.
void SAM2695Synth::setNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    uint8_t CMD_NOTE_OFF[] = {(uint8_t)(MIDI_COMMAND_OFF | (channel & 0x0f)),
                              pitch, 0x00};
    sendCMD(CMD_NOTE_OFF, sizeof(CMD_NOTE_OFF));
}

// Sends a MIDI "All Notes Off" message to stop all notes on a specific MIDI channel
// This command turns off all active notes on the given MIDI channel by sending a control change 
// message with controller number 123 (All Notes Off) and value 0 (to turn off the notes).
// Parameters:
//   channel - The MIDI channel (0-15) on which to send the "All Notes Off" message.
void SAM2695Synth::setAllNotesOff(uint8_t channel)
{
    uint8_t CMD_CONTROL_CHANGE[] = {
        (uint8_t)(MIDI_CMD_CONTROL_CHANGE | (channel & 0x0f)), 0x7b, 0x00};
    sendCMD(CMD_CONTROL_CHANGE, sizeof(CMD_CONTROL_CHANGE));
}

// Sets the pitch value for the synthesizer.
// This value will be used when the pitch is not explicitly provided during note playback.
// Parameters:
//   pitch - The MIDI pitch value (0-127) to be set as the default pitch for the synthesizer.
void SAM2695Synth::setPitch(uint8_t pitch)
{
    _pitch = pitch;
}

uint8_t SAM2695Synth::getPitch() const
{
    return _pitch;
}

// Sets the velocity value for the synthesizer.
// This value will be used as the default velocity when playing notes.
// Parameters:
//   velocity - The MIDI velocity value (0-127) to be set as the default velocity for the synthesizer.
void SAM2695Synth::setVelocity(uint8_t velocity)
{
    _velocity = velocity;
}

// Increases the pitch value by 1, ensuring it does not exceed the maximum allowed pitch.
// If the pitch exceeds the predefined maximum value (BANK0_FX4atmosphere), 
// it is set to that maximum value.
// This function ensures the pitch stays within the valid range.
// No parameters are needed.
void SAM2695Synth::increasePitch()
{
    _pitch++;
    if(_pitch > NOTE_C8) _pitch = NOTE_C8;
    setNoteOn(CHANNEL_10,_pitch,_velocity);
}

void SAM2695Synth::decreasePitch()
{
    _pitch--;
    if(_pitch < NOTE_B0) _pitch = NOTE_B0;
    setNoteOn(CHANNEL_10,_pitch,_velocity);
}

// Decreases the pitch value by 1, ensuring it does not go below the minimum allowed pitch.
// If the pitch goes below the predefined minimum value (BANK1_Clav), 
// it is set to that minimum value.
// This function ensures the pitch stays within the valid range.
// No parameters are needed.
void SAM2695Synth::increaseVelocity()
{
    _velocity += VELOCITY_STEP;
    if(_velocity > VELOCITY_MAX) _velocity = VELOCITY_MAX;
}

void SAM2695Synth::decreaseVelocity()
{
    _velocity -= VELOCITY_STEP;
    if(_velocity < VELOCITY_MIN) _velocity = VELOCITY_MIN;
}

// Decreases the velocity value by a defined step (VELOCITY_STEP), ensuring it does not go below the minimum allowed velocity.
// If the velocity goes below the predefined minimum value (VELOCITY_MIN), 
// it is set to that minimum value.
// No parameters are needed.
void SAM2695Synth::increaseBpm()
{
    setBpm(_bpm + BPM_STEP);
}

// Decreases the beats per minute (BPM) by a defined step (BPM_STEP).
// The new BPM value is calculated by subtracting BPM_STEP from the current BPM value, 
// and then the setBpm function is called to apply the updated BPM.
// No parameters are needed.
void SAM2695Synth::decreaseBpm()
{
    setBpm(_bpm - BPM_STEP);
}

// Sets the beats per minute (BPM) to the specified value, ensuring it stays within the valid range (40 to 240).
// If the provided BPM is less than the minimum allowed value (BPM_MIN, 40), it is set to BPM_MIN.
// If the provided BPM is greater than the maximum allowed value (BPM_MAX, 240), it is set to BPM_MAX.
// Otherwise, the BPM is set to the specified value.
// The valid range for BPM is between 40 and 240.
void SAM2695Synth::setBpm(uint8_t bpm)
{
    if(bpm < BPM_MIN)
        bpm = BPM_MIN;
    else if(bpm > BPM_MAX)
        bpm = BPM_MAX;
    _bpm = bpm;
}

// Returns the current beats per minute (BPM) value.
// This function retrieves the current BPM value stored in the _bpm variable.
// The BPM value is returned as an unsigned 8-bit integer (uint8_t).
uint8_t SAM2695Synth::getBpm() const
{
    return _bpm;
}

// Sends a command to the serial interface.
// This function sends a byte array (cmd) of a specified length (len) through the serial port.
// It uses the _serial object's write method to transmit the command data.
// Parameters:
// - cmd: A pointer to a byte array containing the command data to be sent.
// - len: The length of the byte array to be sent.
void SAM2695Synth::sendCMD(byte* cmd, int len)
{
    _serial->write(cmd, len);
}
