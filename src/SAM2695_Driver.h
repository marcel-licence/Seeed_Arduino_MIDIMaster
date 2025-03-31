// ---------------------------------------------------------------------------
//
// SAM2695Driver.h
// A generic MIDI sequencer library for Arduino.
// Copyright: (c) 2015 Adafruit Industries
// License: GNU GPLv3
// // ---------------------------------------------------------------------------
#ifndef _SAM2695_Driver_H
#define _SAM2695_Driver_H

#include "Arduino.h"
#include "SAM2695_Def.h"


class SAM2695_Driver
{
public:
  SAM2695_Driver(int memory = DEFAULT_MEMORY, HardwareSerial* serial = &Serial2,int baud = 31250,uint8_t RX = 42, uint8_t TX = 43);
  void sendCMD(byte* buf, int size);
  void  begin(int bpms = DEFAULT_BPM, int steps = DEFAULT_STEPS);
  void  run();
  void  setBpm(int tempo);
  int   getBpm();
  void  setSteps(int steps);
  void  setMidiHandler(MIDIcallback cb);
  void  setStepHandler(StepCallback cb);
  void  setNote(byte channel, byte pitch, byte velocity, byte step = -1);
  // byte  getPosition();
  StepNote* getStepNote();

  //todo add test
  void  setMidiHandler2(MIDIcallBack2 cb);

private:
  int               _quantizedPosition();
  int               _greater(int first, int second);
  void              _init(int memory);
  void              _heapSort();
  void              _siftDown(int root, int bottom);
  void              _resetSequence();
  void              _loopPosition();
  void              _tick();
  void              _step();
  void              _triggerNotes();

private:
  MIDIcallback      _midiCallBack;
  StepCallback      _stepCallBack;
  StepNote*  _sequence;

  //todo add test
  MIDIcallBack2     _midiCallBack2;

  bool              _running;
  int               _sequenceSize;
  int               _bpm;
  byte              _steps;
  byte              _position;
  unsigned long     _clock;
  unsigned long     _sixteenth;
  unsigned long     _shuffle;
  unsigned long     _nextBeat;
  unsigned long     _nextClock;
public:
  HardwareSerial*   _serial;
};

#endif
