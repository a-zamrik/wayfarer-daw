# Wayfarer Digital Audio Workstation
A successor of a previously made digital audio workstation (DAW) written in python dubbed "voice to text to speech synth (vttts synth)".
vttts synth was originally an audio editing exploration project that slowly grew into a DAW.
vttts synth was a faceless DAW controlled through voice commands.
It allowed for audio busses, audio effects, and synthesized speech to be controlled through configurable voice commands.

Wayfarer is the successor of vttts Synth; writen in a more apt language for real-time audio processing and interacted via a GUI instead of spoken words.


![Wayfarer](img/Simple%20Overview.png)

## Features
* A simple sine synth that can be played through the keyboard
    * ADSR, and Gain are configurable
* WAV files may be loaded and played
* Audio Effect Chains
    * This affects both the Sine Synth and any audio currently being played.
    Audio effects may be added, removed, or re-ordered in real time.
* A POC GUI utilizing the DearImGui framework.
* Pre-sequenced midi notes may be played on repeat. (Not accessible through GUI)
* Multiple bus tracks; each track having an associated instrument and audio effect chain. (Not accessible through GUI)

## Available Instruments
A multi-voice sine synth is provided, it can be played through keyboard controls. Attack time, decay time, attack amplitude, sustain amplitude, and release time are configurable.

[![Midi Note Showcase](https://img.youtube.com/vi/JvfmRe61hF0/maxresdefault.jpg)](https://youtu.be/JvfmRe61hF0)

## Available Audio Effects
#### Auto Filter
Similar to Ableton's Auto Filter, Allows for Low Pass, High Pass, Band Pass, and Notch Filters.
Target frequency and Q of the filter may be changed through the GUI.

[![Audio Effect Showcase](https://img.youtube.com/vi/kmuEwH7KUCU/maxresdefault.jpg)](https://youtu.be/kmuEwH7KUCU)

## Audio Effects to be ported from VTTTS Synth
#### Micro-cassette emulator
#### Reverb 
#### Phase vocoder effects such as; vocoder, pitch shifter, robotic vocoder
#### Paul stretch

## Keyboard Controls
Piano notes C to B map to keyboard keys **a w s e d f t g y h j k**.
Piano note C maps to keyboard key A.
Piano note C# maps to keyboard key W.
Piano note D maps to keyboard key S.
And so on...

The octave of each note may be changed using **z x**, where **z** will decrease the octave, and **x** will increase.



## Building First Time
```bash
wayfarer-daw> mkdir build; cd build
wayfarer-daw\build> cmake -DUSE_IMGUI:BOOL=ON ..
wayfarer-daw\build> cd ..   
```

## Compiling
```bash 
wayfarer-daw\> cmake --build .\build  
```

## Running
```bash
wayfarer-daw> .\build\Debug\wayfarer.exe
```

## Next Steps
* Working on loading and reading midi files to play precomposed songs.
* Working on multi bus track GUI allowing for multiple and different instruments to be played simultaneously.
* Working on midi note sequence GUI editor to compose music.