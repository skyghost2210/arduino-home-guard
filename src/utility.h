#ifndef UTILITY_H
#define UTILITY_H

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Arduino.h>

void update_display(LiquidCrystal &lcd, String &currentString, String newString);

void update_rgb_led(int pinR, int pinG, int pinB, int &currentLedR, int &currentLedG, int &currentLedB, int newR, int newG, int newB);

void update_led_rgb_without_delay(unsigned long &ledChangeMilliTimestamp, unsigned long ledChangeInterval, int pinR, int pinG, int pinB, int &currentLedR, int &currentLedG, int &currentLedB, int firstR, int firstG, int firstB, int secondR, int secondG, int secondB);

String get_text_EEPROM(int START_ADDRESS);

void save_text_EEPROM(String text, int START_ADDRESS);

String get_text_in_middle_row(String text);

String get_inputted_passcode(LiquidCrystal &lcd, String &currentDisplayText, Keypad &keypad, String &currentString, unsigned int length, String questionString);

#endif /* UTILITY_H */