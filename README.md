# Seeed Arduino MIDIMaster

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)

## 💫 Project Overview

Seeed Arduino MIDIMaster is a professional-grade MIDI synthesizer development library designed specifically for the XIAO Serial microcontroller paired with the SAM2695 audio synthesis chip. This library provides comprehensive MIDI control capabilities, enabling creators, musicians, and developers to easily build complex musical projects, from simple monophonic melodies to multi-track chord performances.

![XIAO MIDI Synth Board](https://files.seeedstudio.com/wiki/XIAO_Series/MIDISynth.jpg)

## ✨ Core Features

### Sound & Expression
- **Professional Sound Library** - Built-in collection of 127 high-quality instrument sounds spanning pianos, strings, brass, percussion, and more
- **Pitch Control** - Precise scale adjustment system with support for real-time semitone shifting
- **Volume Dynamics** - Programmable volume control with 127 levels of fine adjustment
- **Tempo Adjustment** - Flexible BPM control for music rhythm variations

### Advanced Playback Features
- **16-Channel Multi-track Playback** - Support for controlling 16 independent MIDI channels simultaneously for complex arrangements
- **Chord Synthesis** - Advanced chord playback with support for arbitrary note combinations and chord complexity control
- **Professional Drum Kit** - Channel 9 dedicated drum machine functionality with various percussion sounds

### System Architecture
- **LED Status Indication** - Intuitive LED feedback system with different blinking patterns for different modes
- **Event Handling System** - Robust event processing with support for short press, long press and complex interactions
- **Reliable Serial Communication** - Support for both hardware and software serial interfaces, offering multiple connection options
- **State Machine Management** - Efficient state machine design for clear, logical button operation
  - This example implements a complete state machine architecture, which includes the following core components:
  - 1:State Interface and Concrete State Classes 
  - 2:Event System 
  - 3:State Machine Core 
  - 4:Function Registration and Management System 
  - In this example, buttons are used as event triggers for the state machine, supporting button actions such as short press, long press, and release. 
  - This architecture ensures flexibility and scalability:
  - states and events are easy to extend, and new features can be added simply by creating new state classes and registering them. 
  - Event handling and state transitions are separated. 
  - You can design and extend the functionality of the state machine for secondary development based on this architecture.

## 🔧 Hardware Requirements

- **Processor:** Seeed XIAO Serial microcontroller
- **Synthesis Chip:** SAM2695 audio synthesis driver board
- **Connection:** XIAO expansion board (optional)
- **Power:** USB power or 3.7V lithium battery

## 📦 Installation Guide

### Arduino IDE Installation
```
1. Open Arduino IDE，eg: XIAO ESP32S3
2. Select Tools > Board Manager, install "Seeed XIAO ESP32S3"
3. Select Sketch > Include Library > Manage Libraries
4. Search for and install the "Seeed_Arduino_MIDIMaster" library
5. For software serial functionality, also install "EspSoftwareSerial" library
```

### PlatformIO Installation
```ini
# e.g. XIAO ESP32S3
# platformio.ini
[env:seeed_xiao_esp32s3]
platform = espressif32
board = seeed_xiao_esp32s3
framework = arduino
```


## 📝 Usage Examples

### Basic MIDI Note Playing
```cpp
#include <SAM2695Synth.h>

#define COM_SERIAL Serial0
#define SHOW_SERIAL Serial
SAM2695Synth<HardwareSerial> synth = SAM2695Synth<HardwareSerial>::getInstance();

void setup()
{
  //serial init to usb
  SHOW_SERIAL.begin(USB_SERIAL_BAUD_RATE);
  // Synth initialization. Since a hardware serial port is used here, the software serial port is commented out.
  synth.begin(COM_SERIAL, MIDI_SERIAL_BAUD_RATE);
  synth.setInstrument(0,CHANNEL_0,unit_synth_instrument_t::GrandPiano_1);
  delay(1000);
}

void loop()
{
  synth.setNoteOn(CHANNEL_0,NOTE_E4,VELOCITY_DEFAULT);
  delay(1000);
  synth.setNoteOff(CHANNEL_0,NOTE_E4);
  delay(1000);
}
```
### Basic State Machine Example
```cpp
#include <Event.h>
#include <State.h>
#include <StateMachine.h>
#include <StateManager.h>

Event* event getNextEvent();
void errorHandler(int errorCode, const char* errorMsg);

//create state machine
StateMachine stateMachine;
StateManager* manager = StateManager::getInstance();

class newState : public State{// Concrete Implementation Class Methods};
class ErrorState : public State{// Concrete Implementation Class Methods};

void setup()
{
    //regist three mode state
    manager->registerState(new newState());
    //regist error state
    ErrorState* errorState = new ErrorState();
    manager->registerState(errorState);
    //set error handler
    stateMachine.setErrorHandler(errorHandler);
    //init state machine
    if(!(stateMachine.init(manager->getState(newState::ID), errorState)))
    {
        StateManager::releaseInstance();
        return ;
    }
}

void loop()
{
    Event* event = getNextEvent();
    if(event != nullptr)
    {
        stateMachine.handleEvent(event);
        // set the event type is None
        stateMachine.recycleEvent(event);
    }
}

Event* event getNextEvent()
{
    // Concrete Implementation Methods
    return nullptr;
}

void errorHandler(int errorCode, const char* errorMsg) 
{
    printf("error: [%d] %s\n", errorCode, errorMsg);
}

```



## 📚 API Documentation Summary

- `begin(T& serial , int baud);` - function is used to initialize the communication with a specified serial port. The template parameter `T` can be a software serial port or a hardware serial port. This flexibility allows users to select either a software-based or hardware-based serial interface depending on their system setup.
- `setInstrument(bank, channel,value);` - Set the instrument sound for specified channel
- `noteOn(channel, note, velocity)` - Trigger specified note
- `noteOff(channel, note)` - Stop specified note
- `playChord(chord)` - Play specified chord, with support for arbitrary note combinations and chord complexity control
- `increaseBpm()/decreaseBpm()` - Adjust music Tempo
- `increasePitch()/decreasePitch()` - Adjust pitch
- `increaseVelocity()/decreaseVelocity()` - Adjust volume
- See full documentation for more advanced features

## ❓ Frequently Asked Questions

**Q: Is XIAO ESP32C3 supported?**  
A: Yes, this library is compatible with all XIAO series development boards, including ESP32C3.

**Q: How do I connect external audio output?**  
A: The SAM2695 driver board provides a 3.5mm audio output that can be directly connected to headphones or powered speakers.

**Q: How many notes can be played simultaneously?**  
A: The SAM2695 chip supports up to 64 polyphonic voices, theoretically allowing up to 64 simultaneous notes. However, a macro is set to allow customization, and the current default is 4.

## 🤝 Contribution Guidelines

Community contributions are welcome! If you'd like to participate in this project:

1. Fork this repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Submit a Pull Request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details

---

**Seeed Studio** | [Official Website](https://www.seeedstudio.com/) | [Technical Support](https://forum.seeedstudio.com/)
