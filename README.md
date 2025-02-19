# Wayfarer Digital Audio Workstation
A successor of a previously made DAW written in python dubbed "vttts synth".
vttts synth was orignaly an audio editing exploration project that slowly growed into a DAW.
vttts synth was a faceless DAW controlled through voice commands.
It allowed for audio busses, effects, and synthesized speech to be controlled through configurable voice commands.

Wayfarer was intended to be the successor of VTTTS Synth; in which it is written in C++ (ideal for realtime applications), and it is interacted with a GUI instead of spoken words.

It is very much a proof of concept currently.

![Wayfarer](img/Simple%20Overview.png)

## Features
* A simple sine synth that can be played through the keyboard
    * ADSR, and Gain are configurable
* WAV files may be loaded and played
* Audo Effect Chains
    * This affects both the Sine Synth and any audio currently being played.
    Audio effects may be added, removed, or re-ordered in real time.
* A POC GUI utilizing the DearImGui framework.

## Available Audio Effects
#### Auto Filter
    Similar to Ableton's Auto Filter, Allows for Low Pass, High Pass, Band Pass, and Notch Filters.

## Audio Effects to be ported from VTTTS Synth
#### Micro-cassette emulator
#### Reverb
#### Phase vocoder effects such as; vocoder, pitch shifter, robotic vocoder
#### Paul streatch

## Building
```bash
wayfarer-daw> cmake --build .\build\    
```

## Keyboard Controls
Piano notes C to B map to keyboard keys **a w s e d f t g y h j k**.
Piano note C maps to keyboard key A.
Piano note C# maps to keyboard key W.
Piano note D maps to keyboard key S.
And so on...

The octave of each note may be changed using **z x**, where **z** will decrease the octave, and **x** will increase.