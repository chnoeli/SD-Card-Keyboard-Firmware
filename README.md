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

## Setting up the Makropad with multiple config files

1. Flash the Firmware to the macropad
2. Create files on the SD card `config_0.txt` 0 is stating that it is the first config file `1` for the second `2` for the third etc. (Currently the limit is 255)
3. Copy the `TMF.txt`
    1. Make sure that `CSTM_NEXT_CONFIG` is configured to any button action
    2. This button can then be used to rotate the config files when it reaches the last file it loads the `config_0.txt` file.
    3. The `CSTM_PREVIOUS_CONFIG` is not yet working properly as when you hit 0 and hit it it will not wrap to the highest file (WIP)
4. Insert the SD card
5. Start using the Macropad :)

### Important infromation
When there is no SD and the config file is changed it will be stuck in a loop until a SD card is loaded.


## TODO List

-   get deej working with AFSC Desktop Macropad
-   lower program space used even more (current: 24374 bytes (85%))
-   add more comments and documentation
-   Do extensive testing with main branch before making the switch for production macropads.
-   go through TMF file and throw out unused keycodes, add needed keycodes.
-   CONSIDER renaming keycodes in TMF file to be closer to match current [GB2 configuration](https://www.pikatea.com/guides)

## Future Goals and Objectives.

-   overhaul to add layer support (different branch)
