#pragma once

#ifndef _SAM2695_DEF_H
#define _SAM2695_DEF_H

/*------------------------------------------- Defines begin -------------------------------------------*/
#define MIDI_SERIAL_BAUD_RATE           31250   //MIDI串口波特率
#define USB_SERIAL_BAUD_RATE            9600    //USB串口波特率
#define MIDI_COMMAND_ON                 0x90    //MIDI控制开关-开
#define MIDI_COMMAND_OFF                0x80    //MIDI控制开关-关
#define BPM_DEFAULT                     120     //默认BPM
#define BPM_MIN                         40      //最小BPM
#define BPM_MAX                         240     //最大BPM
#define BPM_STEP                        10      //BPM步进
#define MIDI_CMD_CONTROL_CHANGE         0xB0    //control change
#define MIDI_CMD_PROGRAM_CHANGE         0xC0    //program change
#define VELOCITY_MIN                    0       //声音 0~127 0
#define VELOCITY_MAX                    127     //声音 0~127 127
#define VELOCITY_STEP                   10      //声音 0~127 步进为10
#define BASIC_TIME                      60000   //1分钟的毫秒数
#define QUATER_NOTE                     0       //4分音符
#define EIGHTH_NOTE                     1       //8分音符
#define SIXTEENTH_NOTE                  2       //16分音符
#define BEATS_BAR_DEFAULT               4       //默认每小节拍数 - 4/4拍
#define BEATES_BAR_2                    2       //每小节拍数 - 2/4拍
#define BEATS_BAR_3                     3       //每小节拍数 - 3/4拍
#define BEATS_BAR_4                     4       //每小节拍数 - 4/4拍


/* define channel begin */
#define CHANNEL_0                       0
#define CHANNEL_1                       1
#define CHANNEL_2                       2
#define CHANNEL_3                       3
#define CHANNEL_4                       4
#define CHANNEL_5                       5
#define CHANNEL_6                       6
#define CHANNEL_7                       7
#define CHANNEL_8                       8
#define CHANNEL_9                       9
#define CHANNEL_10                      10
#define CHANNEL_11                      11
#define CHANNEL_12                      12
#define CHANNEL_13                      13
#define CHANNEL_14                      14
#define CHANNEL_15                      15

/* define note*/
#define BANK0_Grand_Piano1 1
#define BANK0_Bright_Piano2 2
#define BANK0_ElGrd_Piano3 3
#define BANK0_Honky_tonkPiano 4
#define BANK0_ElPiano1 5
#define BANK0_ElPiano2 6
#define BANK0_Harpsichord 7
#define BANK0_Clavi 8
#define BANK0_Celesta 9
#define BANK0_Glockenspiel 10
#define BANK0_MusicBox 11
#define BANK0_Vibraphone 12
#define BANK0_Marimba 13
#define BANK0_Xylophone 14
#define BANK0_TubularBells 15
#define BANK0_Santur 16
#define BANK0_DrawbarOrgan 17
#define BANK0_PercussiveOrgan 18
#define BANK0_RockOrgan 19
#define BANK0_ChurchOrgan 20
#define BANK0_ReedOrgan 21
#define BANK0_Accordion_French 22
#define BANK0_Harmonica 23
#define BANK0_TangoAccordion 24
#define BANK0_AccousticGuitarNylon 25
#define BANK0_AccousticGuitarSteel 26
#define BANK0_ElGuitarjazz 27
#define BANK0_ElGuitarclean 28
#define BANK0_ElGuitarmuted 29
#define BANK0_OverdrivenGuitar 30
#define BANK0_DistortionGuitar 31
#define BANK0_GuitarHarmonics 32
#define BANK0_AcousticBass 33
#define BANK0_FingerBass 34
#define BANK0_PickedBass 35
#define BANK0_FretlessBass 36
#define BANK0_SlapBass1 37
#define BANK0_SlapBass2 38
#define BANK0_SynthBass1 39
#define BANK0_SynthBass2 40
#define BANK0_Violin 41
#define BANK0_Viola 42
#define BANK0_Cello 43
#define BANK0_Contrabass 44
#define BANK0_TremoloStrings 45
#define BANK0_PizzicatoStrings 46
#define BANK0_OrchestralHarp 47
#define BANK0_Timpani 48
#define BANK0_StringEnsemble1 49
#define BANK0_StringEnsemble2 50
#define BANK0_SynthStrings1 51
#define BANK0_SynthStrings2 52
#define BANK0_ChoirAahs 53
#define BANK0_VoiceOohs 54
#define BANK0_SynthVoice 55
#define BANK0_OrchestraHit 56
#define BANK0_Trumpet 57
#define BANK0_Trombone 58
#define BANK0_Tuba 59
#define BANK0_MutedTrumpet 60
#define BANK0_FrenchHorn 61
#define BANK0_BrassSection 62
#define BANK0_SynthBrass1 63
#define BANK0_SynthBrass2 64
#define BANK0_SopranoSax 65
#define BANK0_AltoSax 66
#define BANK0_TenorSax 67
#define BANK0_BaritoneSax 68
#define BANK0_Oboe 69
#define BANK0_EnglishHorn 70
#define BANK0_Bassoon 71
#define BANK0_Clarinet 72
#define BANK0_Piccolo 73
#define BANK0_Flute 74
#define BANK0_Recorder 75
#define BANK0_PanFlute 76
#define BANK0_BlownBottle 77
#define BANK0_Shakuhachi 78
#define BANK0_Whistle 79
#define BANK0_Ocarina 80
#define BANK0_Lead1square 81
#define BANK0_Lead2sawtooth 82
#define BANK0_Lead3calliope 83
#define BANK0_Lead4chiff 84
#define BANK0_Lead5charang 85
#define BANK0_Lead6voice 86
#define BANK0_Lead7fifths 87
#define BANK0_Lead8bassLead 88
#define BANK0_Pad1fantasia 89
#define BANK0_Pad2warm 90
#define BANK0_Pad3polysynth 91
#define BANK0_Pad4choir 92
#define BANK0_Pad5bowed 93
#define BANK0_Pad6metallic 94
#define BANK0_Pad7halo 95
#define BANK0_Pad8sweep 96
#define BANK0_FX1rain 97
#define BANK0_FX2soundtrack 98
#define BANK0_FX3crystal 99
#define BANK0_FX4atmosphere 100
#define BANK0_FX5brightness 101
#define BANK0_FX6goblins 102
#define BANK0_FX7echoes 103
#define BANK0_FX8sci_fi 104
#define BANK0_Sitar 105
#define BANK0_Banjo 106
#define BANK0_Shamisen 107
#define BANK0_Koto 108
#define BANK0_Kalimba 109
#define BANK0_Bagpipe 110
#define BANK0_Fiddle 111
#define BANK0_Shanai 112
#define BANK0_TinkleBell 113
#define BANK0_Agogo 114
#define BANK0_SteelDrums 115
#define BANK0_Woodblock 116
#define BANK0_TaikoDrum 117
#define BANK0_MelodicTom 118
#define BANK0_SynthDrum 119
#define BANK0_ReverseCymbal 120
#define BANK0_GuitarFretNoise 121
#define BANK0_BreathNoise 122
#define BANK0_Seashore 123
#define BANK0_BirdTweet 124
#define BANK0_TelephRing 125
#define BANK0_Helicopter 126
#define BANK0_Applause 127
#define BANK0_Gunshot 128


/* define pitch */
#define BANK1_Piano1 1
#define BANK1_Piano2 2
#define BANK1_Piano3 3
#define BANK1_DetunedEP1 4
#define BANK1_EPiano1 5
#define BANK1_EPiano2 6
#define BANK1_DetunedEP2 7
#define BANK1_Honky_Tonk 8
#define BANK1_Organ1 9
#define BANK1_Organ2 10
#define BANK1_Organ3 11
#define BANK1_DetunedOr1 12
#define BANK1_ChurchOrg2 13
#define BANK1_ChurchOrg 14
#define BANK1_ChurchOrg 15
#define BANK1_AccordionFr 16
#define BANK1_Harpsichord 17
#define BANK1_CoupledHps 18
#define BANK1_CoupledHps 19
#define BANK1_Clav 20
#define BANK1_Clav 21
#define BANK1_Clav 22
#define BANK1_Celesta 23
#define BANK1_Celesta 24
#define BANK1_SynthBrass1 25
#define BANK1_SynthBrass2 26
#define BANK1_SynthBrass3 27
#define BANK1_SynthBrass4 28
#define BANK1_SynthBass1 29
#define BANK1_SynthBass2 30
#define BANK1_SynthBass3 31
#define BANK1_SynthBass4 32
#define BANK1_Fantasia 33
#define BANK1_SynCalliope 34
#define BANK1_ChoirAahs 35
#define BANK1_BowedGlass 36
#define BANK1_Soundtrack 37
#define BANK1_Atmosphere 38
#define BANK1_Crystal 39
#define BANK1_BagPipe 40
#define BANK1_TinkleBell 41
#define BANK1_IceRain 42
#define BANK1_Oboe 43
#define BANK1_PanFlute 44
#define BANK1_SawWave 45
#define BANK1_Charang 46
#define BANK1_TubularBells 47
#define BANK1_SquareWave 48
#define BANK1_Strings 49
#define BANK1_TremoloStr 50
#define BANK1_SlowStrings 51
#define BANK1_PizzicatoStr 52
#define BANK1_Violin 53
#define BANK1_Viola 54
#define BANK1_Cello 55
#define BANK1_Cello 56
#define BANK1_Contrabass 57
#define BANK1_Harp 58
#define BANK1_Harp 59
#define BANK1_Nylon_strGt 60
#define BANK1_Steel_StrGt 61
#define BANK1_ChorusGt 62
#define BANK1_FunkGt 63
#define BANK1_Sitar 64
#define BANK1_AcousticBs 65
#define BANK1_FingeredBs 66
#define BANK1_PickedBs 67
#define BANK1_FretlessBs 68
#define BANK1_SlapBs1 69
#define BANK1_SlapBs2 70
#define BANK1_FretlessBs 71
#define BANK1_FretlessBs 72
#define BANK1_Flute 73
#define BANK1_Flute 74
#define BANK1_Piccolo 75
#define BANK1_Piccolo 76
#define BANK1_Recorder 77
#define BANK1_PanFlute 78
#define BANK1_SopranoSax 79
#define BANK1_AltoSax 80
#define BANK1_TenorSax 81
#define BANK1_BaritoneSax 82
#define BANK1_Clarinet 83
#define BANK1_Clarinet 84
#define BANK1_Oboe 85
#define BANK1_EnglishHorn 86
#define BANK1_Bassoon 87
#define BANK1_Harmonica 88
#define BANK1_Trumped 89
#define BANK1_MutedTrumpet 90
#define BANK1_Trombone 91
#define BANK1_Trombone 92
#define BANK1_FrenchHorn 93
#define BANK1_FrenchHorn 94
#define BANK1_Tuba 95
#define BANK1_Brass 96
#define BANK1_Brass2 97
#define BANK1_Vibraphone 98
#define BANK1_Vibraphone 99
#define BANK1_Kalimba 100
#define BANK1_TinkleBell 101
#define BANK1_Glockenspiel 102
#define BANK1_Tubular_Bell 103
#define BANK1_Xylophone 104
#define BANK1_Marimba 105
#define BANK1_Koto 106
#define BANK1_TaishoKoto 107
#define BANK1_Shakuhachi 108
#define BANK1_Whistle 109
#define BANK1_Whistle 110
#define BANK1_BottleBlow 111
#define BANK1_PanFlute 112
#define BANK1_Timpani 113
#define BANK1_MeloTom 114
#define BANK1_MeloTom 115
#define BANK1_SynthDrum 116
#define BANK1_SynthDrum 117
#define BANK1_Taiko 118
#define BANK1_Taiko 119
#define BANK1_ReverseCym 120
#define BANK1_Castanets 121
#define BANK1_TinkleBell 122
#define BANK1_OrchestraHit 123
#define BANK1_Telephone 124
#define BANK1_Bird 125
#define BANK1_Helicopter 126
#define BANK1_BowedGlass 127
#define BANK1_IceRain 128
/* Drum effects (channel 10) */
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