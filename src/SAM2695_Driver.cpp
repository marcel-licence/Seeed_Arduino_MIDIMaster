
#include "Arduino.h"
#include "SAM2695_Driver.h"


/**
 * Constructor for the SAM2695_Driver class.
 * Initializes the driver with the specified amount of memory.
 * If no memory value is provided, it defaults to 512.
 *
 * @param memory The amount of memory to allocate for the driver.
 *               Defaults to 512 if not specified.
 */
SAM2695_Driver::SAM2695_Driver(int memory)
{
  _init(memory);
}

/**
 * Initializes the SAM2695_Driver with the specified BPM (beats per minute) and number of steps.
 * This function sets up the metronome's timing and the number of steps in the sequence.
 *
 * @param bpm The beats per minute for the metronome, controlling the tempo.
 * @param steps The number of steps in the sequence (e.g., number of beats or patterns).
 *
 * @return void
 */
void SAM2695_Driver::begin(int bpm, int steps)
{
  // Set the BPM (tempo) for the metronome
  setBpm(bpm);
  // Set the number of steps in the sequence
  setSteps(steps);
}



/**
 * IMPORTANT: This method should only be called in the
 * main loop of the sketch, and is required for the
 * sequencer to work.
 *
 * This method checks the current time against the time of the
 * next scheduled beat and steps the progression forward if the
 * current time is equal to or greater than the next beat time.
 *
 * @param void
 *
 * @return void
 */
void SAM2695_Driver::run()
{

  if(! _running)
    return;

  // what's the time?
  unsigned long now = millis();
  // it's time to get ill.

  // send clock
  if(now >= _nextClock) {
    _tick();
    _nextClock = now + _clock;
  }

  // only step if it's time
  if(now < _nextBeat)
    return;

  // advance and send notes
  _step();

  // add shuffle offset to next beat if needed
  if((_position % 2) == 0)
    _nextBeat = now + _sixteenth + _shuffle;
  else
    _nextBeat = now + _sixteenth - _shuffle;

}

/**
 * Sets the beats per minute (BPM) for the SAM2695_Driver.
 * The BPM value is clamped to be within the defined minimum and maximum tempo limits.
 *
 * @param tempo The desired tempo (BPM) to be set.
 *              If the tempo is below the minimum, it will be set to the minimum value.
 *              If the tempo exceeds the maximum, it will be set to the maximum value.
 *
 * @return void
 */
//todo the specific implementation of modifying the metronome will be done later
void SAM2695_Driver::setBpm(int tempo)
{
  // Ensure tempo is not below the minimum
  if(tempo < MIN_TEMPO)
    tempo = MIN_TEMPO;
  // Ensure tempo is not above the maximum
  if(tempo > MAX_TEMPO)
    tempo = MAX_TEMPO;
  _bpm = tempo;
}

/**
 * Returns the current beats per minute (BPM) value.
 * This function retrieves the BPM that has been set for the metronome.
 *
 * @param void
 *
 * @return The current BPM value.
 */
int SAM2695_Driver::getBpm()
{
  // Return the current BPM value
  return _bpm;
}


/**
 * Sets the number of steps in the sequence for the SAM2695_Driver.
 * The number of steps is clamped to be within the defined maximum step limit.
 * It also resets any notes in the sequence that are beyond the current step count.
 *
 * @param steps The desired number of steps to set in the sequence.
 *              If the number of steps exceeds the maximum, it will be set to the maximum value.
 *
 * @return void
 */
void SAM2695_Driver::setSteps(int steps)
{
  if(steps > MAX_STEPS)  // Ensure the steps are not greater than the maximum allowed
    steps = MAX_STEPS;
  _steps = steps;  // Store the valid number of steps

  // Loop through the sequence and reset any notes beyond the current step count
  for(int i = 0; i < _sequenceSize; ++i)
  {
    // Reset steps that are greater than or equal to the current step count
    if(_sequence[i].step >= _steps)
      _sequence[i] = DEFAULT_NOTE;
  }
}

/**
 * IMPORTANT: Setting a MIDI handler is required for the sequencer to operate
 * Allows user to set a callback that will be called when
 * the sequencer needs to send MIDI messages. Using this
 * callback allows the sequencer to be decoupled from the
 * MIDI implementation.
 * The callback will be called with three arguments: the MIDI
 * command, the first argument for that MIDI command, and the
 * second argument for that command. For example, if the callback
 * was called with a note on message, the arguments would be the
 * note on command (0x9), the pitch value (0x3C), and the velocity
 * value (0x40). The user is responsible for passing those values
 * to the specific MIDI library they are using. Please check the
 * typedef for MIDIcallback in SAM2695_Driver.h for more info about
 * the arguments.
 *
 * @param cb the midi callback that the sequencer will use
 *
 * @return void
 */
void SAM2695_Driver::setMidiHandler(MIDIcallback cb)
{
  _midiCallBack = cb;
}

/**
 * Allows user to specifiy a callback that will be called
 * whenever the progression steps forward. The callback will
 * be called with the current position as the first argument,
 * and the previous position as the second argument. Please check
 * the typedef for StepCallback in SAM2695_Driver.h for more info
 * about the arguments passed to the callback.
 *
 * @param cb the callback function to call when the progression advances
 *
 * @return void
 */

void SAM2695_Driver::setStepHandler(StepCallback cb)
{
  _stepCallBack = cb;
}

/**
 * Set a note on or off at the current step position
 * This function allows the user to set a note on or off at a specified step position in the sequence.
 * If there is already a note on value at this position, the note will be turned off.
 * The function takes into account the MIDI channel, pitch, velocity, and step to update the note status.
 *
 * @param channel The MIDI channel on which the note is being played.
 * @param pitch The pitch of the note (MIDI note number).
 * @param velocity The velocity of the note (how hard the note is played).
 * @param step The current step position where the note should be set.
 *
 * @return void
 */
void SAM2695_Driver::setNote(byte channel, byte pitch, byte velocity, byte step)
{
  if(! _running)
    return;
  int position;


  bool added = false;

  if(step == -1)
    position = _quantizedPosition();
  else
    position = step;

  for(int i = _sequenceSize - 1; i >= 0; i--)
  {

    if(_sequence[i].pitch > 0 && _sequence[i].pitch != pitch)
      continue;

    if(_sequence[i].step != position && _sequence[i].pitch != 0)
      continue;

    if(_sequence[i].channel != channel && _sequence[i].pitch != 0)
      continue;

    if(_sequence[i].pitch == pitch && _sequence[i].step == position && _sequence[i].channel == channel)
    {

      if(velocity > 0 && _sequence[i].velocity > 0) {
        _sequence[i] = DEFAULT_NOTE;
        added = true;
        continue;
      } else if(velocity == 0 && _sequence[i].velocity == 0) {
        _sequence[i] = DEFAULT_NOTE;
        added = true;
        continue;
      }

    }

    // use the free slot
    if(_sequence[i].pitch == 0 && _sequence[i].step == 0 && _sequence[i].channel == 0 && !added)
    {

      _sequence[i].channel = channel;
      _sequence[i].pitch = pitch;
      _sequence[i].velocity = velocity;
      _sequence[i].step = position;

      added = true;
    }
  }
  _heapSort();
}

/**
 *Returns a pointer to the current sequence
 *
 * @param void
 *
 * @return StepNote*
 */
StepNote* SAM2695_Driver::getStepNote()
{
  return _sequence;
}

/**
 *
 */
//! add a new function to test
void SAM2695_Driver::setMidiHandler2(MIDIcallBack2 cb)
{
  _midiCallBack2 = cb;
}

/**
 * Returns the closest 16th note to the
 * present time. This is used to see where to save the new note.
 *
 * @param void
 *
 * @return byte
 */
byte SAM2695_Driver::getPosition()
{
  return _quantizedPosition();
}

/**
 * A common init method for the constructors to
 * use when the class is initialized. Lowering the
 * amount of memory the sequencer uses will effect the
 * amount of polyphony the sequencer will support. By
 * default the sequencer allocates 1k of sram.
 *
 * @param memory the amount of sram to use in bytes
 *
 * @return void
 */
void SAM2695_Driver::_init(int memory)
{
  _running = true;
  _nextBeat = 0;
  _nextClock = 0;
  _position = 0;
  _sequenceSize = memory / sizeof(StepNote);
  _sequence = new StepNote[_sequenceSize];

  // set up default notes
  _resetSequence();
}

/**
 * Sets sequence to default state
 *
 * @param void
 *
 * @return void
 */
void SAM2695_Driver::_resetSequence()
{
  // set sequence to default note value
  for(int i=0; i < _sequenceSize; ++i)
    _sequence[i] = DEFAULT_NOTE;
}

/**
 * Returns the closest 16th note to the
 * present time. This is used to see where to
 * save the new note.
 *
 * @param void
 *
 * @return int
 */
int SAM2695_Driver::_quantizedPosition()
{

  if(_shuffle > 0)
    return _position;

  // what's the time?
  unsigned long now = millis();

  // calculate value of 32nd note
  unsigned long thirty_second = _sixteenth / 2;

  // use current position if below middle point
  if(now <= (_nextBeat - thirty_second))
    return _position;

  // return first step if the next step
  // is past the step count
  if((_position + 1) >= _steps)
    return 0;

  // return next step
  return _position + 1;
}

/**
 * Moves _position forward by one step, calls the
 * step callback with the current & last step position,
 * and triggers any notes at the current position.
 *
 * @param void
 *
 * @return void
 */
void SAM2695_Driver::_step()
{

  // save the last position so we
  // can provide it to the callback
  int last = _position;

  // increment the position
  _position++;

  // start over if we've reached the end
  if(_position >= _steps)
    _position = 0;

  // tell the callback where we are
  // if it has been set by the sketch
  if(_stepCallBack)
    _stepCallBack(_position, last);

  // trigger next set of notes
  _triggerNotes();

}

/**
 * Calls the user defined MIDI callback with the midi clock message
 *
 * @param void
 *
 * @return void
 */
void SAM2695_Driver::_tick()
{

  // bail if the midi callback isn't set
  if(! _midiCallBack)
    return;

  // tick
  _midiCallBack(0x0, 0xF8, 0x0, 0x0);

}

/**
 * Calls the user defined MIDI callback with the position of playback
 *
 * @param void
 *
 * @return void
 */
void SAM2695_Driver::_loopPosition()
{

  // bail if the midi callback isn't set
  if(! _midiCallBack)
    return;

  // send position
  _midiCallBack(0x0, 0xF2, 0x0, _position);

}

/**
 * Sort the sequence based on the heapsort algorithm
 *
 * @param void
 *
 * @return void
 */
void SAM2695_Driver::_heapSort()
{
  int i;
  StepNote tmp;

  for(i = _sequenceSize / 2; i >= 0; i--)
    _siftDown(i, _sequenceSize - 1);

  for(i = _sequenceSize - 1; i >= 1; i--)
  {
    tmp = _sequence[0];
    _sequence[0] = _sequence[i];
    _sequence[i] = tmp;

    _siftDown(0, i - 1);
  }
}

/**
 * Used by heapsort to shift note positions
 *
 * @param root The index of the root element in the heap that needs to be adjusted.
 * @param bottom The index of the bottom element in the heap to define the range for comparison.
 *
 * @return void
 */
void SAM2695_Driver::_siftDown(int root, int bottom)
{

  int max = root * 2 + 1;

  if(max < bottom)
    max = _greater(max, max + 1) == max ? max : max + 1;
  else if(max > bottom)
    return;

  if(_greater(root, max) == root || _greater(root, max) == -1)
    return;

  StepNote tmp = _sequence[root];
  _sequence[root] = _sequence[max];
  _sequence[max] = tmp;

  _siftDown(max, bottom);

}

/**
 * Used by heapsort to compare two notes so we know
 * where they should be placed in the sorted array.
 * Will return -1 if they are equal
 *
 * @param first position to compare
 * @param second position to compare
 *
 * @return int
 */
int SAM2695_Driver::_greater(int first, int second)
{

  if(_sequence[first].velocity > _sequence[second].velocity)
    return first;
  else if(_sequence[second].velocity > _sequence[first].velocity)
    return second;

  if(_sequence[first].pitch > _sequence[second].pitch)
    return first;
  else if(_sequence[second].pitch > _sequence[first].pitch)
    return second;

  if(_sequence[first].step > _sequence[second].step)
    return first;
  else if(_sequence[second].step > _sequence[first].step)
    return second;

  if(_sequence[first].channel > _sequence[second].channel)
    return first;
  else if(_sequence[second].channel > _sequence[first].channel)
    return second;

  return - 1;

}

/**
 * Calls the user defined MIDI callback with
 * all of the note on and off messages at the
 * current step position.
 *
 * @param void
 *
 * @return void
 */
void SAM2695_Driver::_triggerNotes()
{
  // bail if the midi callback isn't set
  if(! _midiCallBack)
    return;
  // loop through the sequence again and trigger note ons at the current position
  for(int i=0; i < _sequenceSize; ++i)
  {
    // ignore if it's not the current position
    if(_sequence[i].step != _position)
      continue;

    // if this position is in the default state, ignore it
    if(_sequence[i].pitch == 0 && _sequence[i].velocity == 0 && _sequence[i].step == 0)
      continue;

    // send note on values to callback
    _midiCallBack(
      _sequence[i].channel,
      _sequence[i].velocity > 0 ? 0x9 : 0x8,
      _sequence[i].pitch,
      _sequence[i].velocity
    );
  }
}
