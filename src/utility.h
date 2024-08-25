#ifndef UTILITY_H
#define UTILITY_H

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Arduino.h>

void update_display(LiquidCrystal &lcd, String &currentString, String newString);

String get_text_EEPROM(int START_ADDRESS);

void save_text_EEPROM(String text, int START_ADDRESS);

String get_text_in_middle_row(String text);

String get_inputted_passcode(LiquidCrystal &lcd, String &currentDisplayText, Keypad &keypad, String &currentString, unsigned int length, String questionString);

#endif /* UTILITY_H */