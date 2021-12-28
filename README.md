# SD-Card-Keyboard-Firmware

Configure your macropad/keyboard with an SD card and text file. No special software required. This is the main firmware used by the company Pikatea

This README is underconstruction. To follow development, join the [Discord Server](https://discord.gg/wJgKSBYz23)

## Setting up the environment

1. Download and install Arduino IDE 2.0.0-beta-.4
2. Clone the Repo into your sketchbook folder (The default arduino sketchbook folder is `Documents/Arduino` on windows) This folder can be configured in the settings of the Arduino IDE
3. Install the boards required. Go to boards manager and search for `Leonardo`. Install the Arduino AVR Boards by Arduino (Currently Version 1.8.3)
4. Install the required libraries. Go to library manager and search and install each of these. (TODO add links to respective Github pages)
    - Encoder by Paul Stoffregen - Version 1.4.2
    - HID-Project by NicoHood - Version 2.8.0
    - Keypad by Mark Stanley, Alexander Brevig - Version 3.1.1
    - SD by Arduino https://github.com/arduino-libraries/SD Currently not available to install through the library manager so you'll have to clone the Repo into the Libraries folder
5. When compiling, make sure Arduino Leonardo is selected
6. That's it! If you have questions with the process don't hestiate to message in the #pikatea-firmware-dev chat on the discord

## TODO List

-   get deej working with AFSC Desktop Macropad
-   lower program space used even more (current: 24374 bytes (85%))
-   add more comments and documentation
-   Do extensive testing with main branch before making the switch for production macropads.
-   go through TMF file and throw out unused keycodes, add needed keycodes.
-   CONSIDER renaming keycodes in TMF file to be closer to match current [GB2 configuration](https://www.pikatea.com/guides)

## Future Goals and Objectives.

-   overhaul to add layer support (different branch)
