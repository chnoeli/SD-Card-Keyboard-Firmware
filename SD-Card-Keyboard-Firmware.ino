// Arduino 1.8.10
// Pikatea SD card Firmware

// libraries to include
#include "HID-Project.h"
#include <Encoder.h>
#include <Keypad.h>
#include <SD.h>
#include <SPI.h>

// Global defines
#define PIKATEA_MACROPAD_GB2
//#define PIKATEA_MACROPAD_AFSC
#define CONFIG_FILE "config.txt"
#define TMF_FILE "TMF.txt"

// Pikatea GB2
#if defined(PIKATEA_MACROPAD_GB2)

#define SD_CSOUT_PIN 13
// the buttons
#define ROWS 1
#define COLS 6
#define BUTTON_AMOUNT 6
char keys[ROWS][COLS] = {{0, 1, 2, 3, 4, 5}};
byte rowPins[ROWS] = {A0};               // connect to the row pinouts of the keypad // change to define
byte colPins[COLS] = {7, 5, 9, 8, 6, 2}; // connect to the column pinouts of the keypad //change to define
Keypad keypad =
    Keypad((char *)(keys), rowPins, colPins, ROWS, COLS); // change to define
String buttonStrings[] = {"", "", "", "", "", "", "", "", "", "", "", ""};
int fakeAnalogSliderValues[] = {1023, 1023, 1023, 1023, 1023, 1023};

// the encoder
#define ENCODER_DO_NOT_USE_INTERRUPTS
const int EncoderPinA[] = {4};
const int EncoderPinB[] = {3};
Encoder myEnc(EncoderPinA[0], EncoderPinB[0]);
#define encoderConstant 3
int deejSensitivityConstant = 32;
String knobStrings[][2] = {{"", ""}};
#endif

// desktop macropad
#if defined(PIKATEA_MACROPAD_AFSC)
// the buttons
#define ROWS 2
#define COLS 5
#define BUTTON_AMOUNT 10
char keys[ROWS][COLS] = {{0, 1, 2, 3, 4},
                         {5, 6, 7, 8, 9}};
byte rowPins[ROWS] = {A3, A0};                                          // connect to the row pinouts of the keypad // change to define
byte colPins[COLS] = {9, 7, 8, 16, 2};                                  // connect to the column pinouts of the keypad //change to define
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // change to define
String buttonStrings[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
int fakeAnalogSliderValues[] = {1023, 1023, 1023, 1023, 1023, 1023};

// the encoder
#define ENCODER_DO_NOT_USE_INTERRUPTS
const int EncoderPinA[] = {4, 5};
const int EncoderPinB[] = {3, 6};
Encoder myEnc(EncoderPinA[0], EncoderPinB[0]);
#define encoderConstant 3
int deejSensitivityConstant = 32;
String knobStrings[][2] = {{"", ""}, {"", ""}};
#endif

// Variables that change

// 0 = standard - 1 = deej
int mode = 0;

// Function definitions
String ExtractSettingWithDefault(String setting, String fileName, String defaultValue = "");

void setup()
{
    Serial.begin(9600);
    //  while (!Serial) {
    //    ; // wait for serial port to connect.
    //  }
    delay(4000); // wait for serial port to connect.
    InitializeSDCard();

    // setup keyboard and keypad
    Keyboard.begin();
    Mouse.begin();
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    keypad.setHoldTime(500);
    pinMode(EncoderPinA[0], INPUT_PULLUP);
    pinMode(EncoderPinB[1], INPUT_PULLUP);
}

unsigned long previousMillis = 0;
unsigned long deejPreviousMillis = 0;
long oldPosition = myEnc.read();
long position = 0;
int currentButton = -1;
boolean dj = false;

void loop()
{
    char key = keypad.getKey();
    unsigned long currentMillis = millis();
    long newPosition = myEnc.read();

    // encoder has turned
    if (newPosition > oldPosition + encoderConstant)
    {
        oldPosition = newPosition;
        // standard mode
        if (!dj)
        {
            pressKeys(knobStrings[0][1], false);
            releaseKeys();
        }
        else
        {
            if (currentButton != -1)
            {
                fakeAnalogSliderValues[currentButton] =
                    fakeAnalogSliderValues[currentButton] - deejSensitivityConstant;
                // Serial.println("adjusting something else and reseting timmer");
                deejPreviousMillis = currentMillis;
            }
        }
    }
    if (newPosition < oldPosition - encoderConstant)
    {
        oldPosition = newPosition;

        // standard mode
        if (!dj)
        {
            pressKeys(knobStrings[0][0], false);
            releaseKeys();
        }
        else
        {
            if (currentButton != -1)
            {
                fakeAnalogSliderValues[currentButton] =
                    fakeAnalogSliderValues[currentButton] + deejSensitivityConstant;
                // Serial.println("adjusting something else and reseting timmer");
                deejPreviousMillis = currentMillis;
            }
        }
    }
}

int holdFlag = 0;
// when a button is pressed
void keypadEvent(KeypadEvent key)
{
    switch (keypad.getState())
    {
    case PRESSED:
        dj = false;
        Serial.println(key);
        break;

    case RELEASED:
        // if the button has not been held
        if (holdFlag == 0)
        {
            // short press
            Serial.print("shortpress: ");
            Serial.println(buttonStrings[(int)key]);
            Keyboard.release(key);
            pressKeys(buttonStrings[(int)key], true);
            releaseKeys();
        }
        holdFlag = 0;
        break;

    case HOLD:
        Serial.print("hold: ");
        Serial.println(buttonStrings[((int)key) + BUTTON_AMOUNT]);
        pressKeys(buttonStrings[((int)key) + BUTTON_AMOUNT], true);
        releaseKeys();
        holdFlag = 1;
        // break;
    }
}

// Initialize the SD card and return 1 if successful
int InitializeSDCard()
{

    Serial.print(F("Initializing SD card..."));
    while (!SD.begin(SD_CSOUT_PIN))
    {
        Serial.println(F("initialization failed!"));
    }
    Serial.println(F("initialization done."));

    // move this to different function
#if defined(PIKATEA_MACROPAD_GB2)
    knobStrings[0][0] = getKeycodes(F("KnobCW="));
    knobStrings[0][1] = getKeycodes(F("KnobCCW="));
    buttonStrings[0] = getKeycodes(F("Button1="));
    buttonStrings[1] = getKeycodes(F("Button2="));
    buttonStrings[2] = getKeycodes(F("Button3="));
    buttonStrings[3] = getKeycodes(F("Button4="));
    buttonStrings[4] = getKeycodes(F("Button5="));
    buttonStrings[5] = getKeycodes(F("KnobButton="));
    buttonStrings[6] = getKeycodes(F("Button1Hold="));
    buttonStrings[7] = getKeycodes(F("Button2Hold="));
    buttonStrings[8] = getKeycodes(F("Button3Hold="));
    buttonStrings[9] = getKeycodes(F("Button4Hold="));
    buttonStrings[10] = getKeycodes(F("Button5Hold="));
    buttonStrings[11] = getKeycodes(F("KnobButtonHold="));
#endif
#if defined(PIKATEA_MACROPAD_AFSC)
    knobStrings[0][0] = getKeycodes(F("Knob1CW="));
    knobStrings[0][1] = getKeycodes(F("Knob1CCW="));
    knobStrings[1][0] = getKeycodes(F("Knob2CW="));
    knobStrings[1][1] = getKeycodes(F("Knob2CCW="));
    buttonStrings[0] = getKeycodes(F("Button1="));
    buttonStrings[1] = getKeycodes(F("Button2="));
    buttonStrings[2] = getKeycodes(F("Button3="));
    buttonStrings[3] = getKeycodes(F("Button4="));
    buttonStrings[4] = getKeycodes(F("Button5="));
    buttonStrings[5] = getKeycodes(F("Button6="));
    buttonStrings[6] = getKeycodes(F("Button7="));
    buttonStrings[7] = getKeycodes(F("Button8="));
    buttonStrings[8] = getKeycodes(F("KnobButton1="));
    buttonStrings[9] = getKeycodes(F("KnobButton2="));
    buttonStrings[10] = getKeycodes(F("Button1Hold="));
    buttonStrings[11] = getKeycodes(F("Button2Hold="));
    buttonStrings[12] = getKeycodes(F("Button3Hold="));
    buttonStrings[13] = getKeycodes(F("Button4Hold="));
    buttonStrings[14] = getKeycodes(F("Button5Hold="));
    buttonStrings[15] = getKeycodes(F("Button3Hold="));
    buttonStrings[16] = getKeycodes(F("Button4Hold="));
    buttonStrings[17] = getKeycodes(F("Button5Hold="));
    buttonStrings[18] = getKeycodes(F("KnobButton1Hold="));
    buttonStrings[19] = getKeycodes(F("KnobButton2Hold="));
#endif
    deejSensitivityConstant = getKeycodes(F("deejSensitivity=")) != "" ? getKeycodes(F("deejSensitivity=")).toInt() : 32;

    if (getKeycodes(F("deejmode=")).indexOf("rue") > 0 or
        getKeycodes(F("deejMode = ")).indexOf("rue") > 0)
    {
        mode = 1;
    }
    return true;
}

String ExtractSettingWithDefault(String setting, String fileName, String defaultValue = "")
{
    File configFile;
    configFile = SD.open(fileName);
    if (configFile)
    {
        char character;
        char previousCharacter = '\n';
        while (configFile.available())
        {
            String line = configFile.readStringUntil('\n');

            if (!line.startsWith(F("//")) && line.length() > 1)
            {
                String settingValue = line.startsWith(setting) ? line.substring(setting.length()) : "";
                settingValue.trim();
                if (settingValue != "")
                {
                    Serial.print(F("From: "));
                    Serial.println(fileName + F(" - ") + setting.substring(0, setting.length() + 1) + F(" : ") + settingValue);
                    configFile.close();
                    return settingValue;
                }
            }
        }
    }
    else
    {
        Serial.print(F("Error opening the file"));
        Serial.println(fileName);
        return "File Error";
    }
    configFile.close();
    return defaultValue;
}

String getKeycodes(String setting)
{
    String value = ExtractSettingWithDefault(setting, CONFIG_FILE);
    if (value.length() > 0)
    {
        String temp = "";
        while (value.lastIndexOf("+") > -1)
        {
            temp.concat(ExtractSettingWithDefault(value.substring(0, value.indexOf("+")) + "=", TMF_FILE, "w(" + value.substring(0, value.indexOf("+")) + ")"));
            temp.concat('+');
            value = value.substring(value.indexOf("+") + 1, value.length());
        }
        temp.concat(ExtractSettingWithDefault(value + "=", TMF_FILE, "w(" + value + ")"));        
        return temp;
    }
}

void pressKey(String given, boolean addDelay)
{
    // if "w(khkjgldf whatever)" is found
    if (given.indexOf(F("w")) >= 0)
    {
        // chop of the first 2 and last 1
        String temp = given.substring(2, given.length() - 1);
        // print out a single char or write out a string
        char c[temp.length() + 1];
        temp.toCharArray(c, sizeof(c));
        if (temp.length() == 1)
        {
            Keyboard.press(c[0]);
        }
        else
        {
            Keyboard.print(c);
        }
    }
    else if (given.toInt() > 1000)
    {
        Consumer.write(ConsumerKeycode(given.toInt() - 1000));
    }
    else
    {
        Keyboard.write(KeyboardKeycode(given.toInt()));
    }
}

void releaseKeys()
{
    //  Serial.println("release All");
    Keyboard.releaseAll();
}

void pressKeys(String given, boolean Delay)
{
    // Serial.println(given);
    // todo remove temp = given;
    String temp = given;
    // if a "+" is found
    while (temp.lastIndexOf("+") > -1)
    {
        pressKey(temp.substring(0, temp.indexOf("+")), Delay);
        temp = temp.substring(temp.indexOf("+") + 1, temp.length());
    }
    pressKey(temp, Delay);
}

unsigned long DJpreviousMillis = 0;
String prevBuiltString = "";

void sendSliderValues()
{
    if (mode == 1)
    {
        String builtString = String("");

        for (int i = 0; i < 5; i++)
        {
            builtString += String((int)fakeAnalogSliderValues[i]);

            if (i < 5 - 1)
            {
                builtString += String("|");
            }
        }
        unsigned long DJcurrentMillis = millis();
        if (DJcurrentMillis - DJpreviousMillis > 10)
        {
            DJpreviousMillis = DJcurrentMillis;
            if (builtString != prevBuiltString)
            {
                Serial.println(builtString);
            }
            prevBuiltString = builtString;
        }
    }
}

void correctSliderValues()
{
    for (int i = 0; i < 5; i++)
    {
        if (fakeAnalogSliderValues[i] < 0)
        {
            fakeAnalogSliderValues[i] = 0;
        }
        if (fakeAnalogSliderValues[i] > 1023)
        {
            fakeAnalogSliderValues[i] = 1023;
        }
    }
}
