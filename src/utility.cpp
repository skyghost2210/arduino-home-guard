#include "utility.h"
#include <Keypad.h>
#include <EEPROM.h>

void update_display(LiquidCrystal &lcd, String &currentDisplayString, String newString)
{
    if (currentDisplayString != newString)
    {
        lcd.clear();                  // Clear the LCD
        unsigned int maxColumns = 16; // Maximum columns for the LCD
        int row = 0;                  // Start at the first row

        for (unsigned int i = 0; i < newString.length(); i += maxColumns)
        {
            String line = newString.substring(i, i + maxColumns); // Get substring for current line
            lcd.setCursor(0, row);                                // Set cursor to the start of the current row
            lcd.print(line);                                      // Print the substring
            row++;                                                // Move to the next row
            if (row >= 2)
            {          // Check if we've exceeded the number of rows
                break; // Exit if there are no more rows
            }
        }
        currentDisplayString = newString;
    }
}

void update_rgb_led(int pinR, int pinG, int pinB, int &currentLedR, int &currentLedG, int &currentLedB, int newR, int newG, int newB)
{
    analogWrite(pinR, newR);
    analogWrite(pinG, newG);
    analogWrite(pinB, newB);
    currentLedR = newR;
    currentLedG = newG;
    currentLedB = newB;
}

void update_led_rgb_without_delay(unsigned long &ledChangeMilliTimestamp, unsigned long ledChangeInterval, int pinR, int pinG, int pinB, int &currentLedR, int &currentLedG, int &currentLedB, int firstR, int firstG, int firstB, int secondR, int secondG, int secondB)
{
    unsigned long currentMillis = millis();
    // Blink without delay
    if (currentMillis - ledChangeMilliTimestamp >= ledChangeInterval)
    {
        ledChangeMilliTimestamp = currentMillis;
        if (currentLedR == secondR && currentLedG == secondG && currentLedB == secondB)
        {
            update_rgb_led(pinR, pinG, pinB, currentLedR, currentLedG, currentLedB, firstR, firstG, firstB);
        }
        else
        {
            update_rgb_led(pinR, pinG, pinB, currentLedR, currentLedG, currentLedB, secondR, secondG, secondB);
        }
    }
}

void buzzing_sound(unsigned long &speakerOnTimestamp, unsigned long speakerOnTimestampInterval, int spakerPin, int pitch, bool &isSpeakerPlaying)
{
    unsigned long currentMillis = millis();
    // Blink without delay
    if (currentMillis - speakerOnTimestamp >= speakerOnTimestampInterval)
    {
        speakerOnTimestamp = currentMillis;
        if (isSpeakerPlaying)
        {
            noTone(spakerPin);
            isSpeakerPlaying = false;
        }
        else
        {
            tone(spakerPin, pitch);
            isSpeakerPlaying = true;
        }
    }
}

String get_text_EEPROM(int START_ADDRESS)
{
    // Read the String
    int strLength = EEPROM.read(START_ADDRESS); // Get the length of the String
    String s = "";
    for (int i = 0; i < strLength; i++)
    {
        char c = EEPROM.read(START_ADDRESS + 1 + i); // Read each character of the String
        s += c;
    }
    return s;
}

void save_text_EEPROM(String text, int START_ADDRESS)
{
    // Save the String
    int strLength = text.length();
    EEPROM.write(START_ADDRESS, strLength); // Save the length of the String
    for (int i = 0; i < strLength; i++)
    {
        EEPROM.write(START_ADDRESS + 1 + i, text[i]); // Save each character of the String
    }
}

String get_text_in_middle_row(String text)
{
    // Calculate the position to start printing the text so it appears in the middle
    int textLength = text.length();
    int totalLength = 16; // Total number of columns in the LCD
    int padding = (totalLength - (textLength)) / 2;
    // Create the padding string
    String paddingString = "";
    for (int i = 0; i < padding; i++)
    {
        paddingString += " ";
    }
    String paddingStringRight = ""; // To fill full 16 slots, for double line

    for (unsigned int i = 0; i < (totalLength - (paddingString.length() + text.length())); i++)
    {
        paddingStringRight += " ";
    }

    return paddingString + text + paddingStringRight;
}

String get_inputted_passcode(LiquidCrystal &lcd, String &currentDisplayText, Keypad &keypad, String &currentString, unsigned int length, String questionString)
{
    if (currentString.length() < length)
    {
        char key = keypad.getKey();
        if (key != NO_KEY)
        {
            currentString.concat(key);
        };
    }
    String starText = "";
    for (unsigned int i = 0; i < length; i++)
    {
        if (i < currentString.length())
        {
            starText += "*";
        }
        else
        {
            starText += " ";
        }
    }
    // Prevent double get middle
    if (questionString.length() < 16)
    {
        questionString = get_text_in_middle_row(questionString);
    }
    update_display(lcd, currentDisplayText, questionString + get_text_in_middle_row("[" + starText + "]"));
    if (currentString.length() == length)
    {
        String passcode = currentString;
        currentString = "";
        return passcode;
    }
    else
    {
        return "";
    }
}