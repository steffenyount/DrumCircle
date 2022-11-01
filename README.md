# The Drum Circle simple sequencer
#### with Hippie AI for Haiku and BeOS R5
***
### About:
This directory contains the version 1.0.4 of the Drum Circle simple sequencer for Haiku and BeOS R5. It was designed and tested on the Intel version of BeOS R4 and ported to PPC by Will Dyson, and later adapted for Haiku by David Karoly.

DrumCircle is my first BeOS app. So many parts of the User Interface are pretty Crude.

DrumCircle is distributed under the Apache License 2.0.

If you like the concept and want to say thank you, please buy me a coffee: https://www.buymeacoffee.com/steffenyount

If you have suggestions, ideas for improvement, or if you would otherwise like to contribute, I'd like to hear about it. Please visit the project on GitHub: https://github.com/steffenyount/DrumCircle

Have fun drumming!

### Usage:
Start the program, and you will see a myriad of buttons and lights. Almost all of them can be clicked on.

| Item             | Description |
| ---              | --- |
| Start/Stop       | Starts and Stops the Sequencer |
| Reset            | Resets the Score |
| Hippie AI        | Toggles the Hippie Artificial Intelligence (AI) on and off |
| BPM              | Sets the number of Beats Per Minute |
| Hits             | Sets the number of hits in (the width of) the active Score |
| HPB              | Sets the number of Hits Per Beat |
| Key/Ins          | Sets the number of Keys or Instruments that make up your active Profile |
| Profiles         | Profiles consist of both the instrument settings and the score |
| The Profile Menu | Allows you to add, remove, load, save, and switch between Profiles |
| Hit%             | Sets the average percentage of hits that should be beats for the Hippie AI |
| Var%             | Sets the average percentage for how often the Hippie AI should Vary the Track |
| Chan             | Sets the channel for that track (Channel 10 is the Drum Channel) |
| Play             | Lets you Play/Mute individual tracks |
| Vol              | Adjusts the attack velocity for the hits of a particular track (Volume) |
| Note             | Adjusts the duration of the hits for a particular track (1/8 note, 1/4 note, etc) |
| Key/Instrument   | Sets the instrument played for chan 10 or the key played for the other channels |
| The Score        | Click on the red dots to edit individual parts of the score |

### Revision History:
1.0.0 First release

1.0.1 Fixed a GUI bug for R5

1.0.2 Fixed a compile bug for R5 and changed the MIME types to comply more with standards

1.0.3 Imported to GitHub, updated license, fixed some spelling, and updated the README

1.0.4 Adapted to run on the Haiku OS. Thank you, David Karoly!