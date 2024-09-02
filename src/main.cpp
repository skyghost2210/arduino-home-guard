#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Keypad.h>
#include <GuardState.h>
#include "utility.h"

/*LCD SETUP*/
LiquidCrystal lcd(12, 11, 10, 13, 14, 15);
String currentDisplayString = "";

/*Motion Sensor SETUP*/
int motionSensorInputPin = 1;      // choose the input pin (for PIR sensor)
int motionState = LOW;             // we start, assuming no motion detected
int currentMotionSensorStatus = 0; // variable for reading the pin status

/*Keypad SETUP*/
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

uint8_t colPins[COLS] = {5, 4, 3, 2}; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = {9, 8, 7, 6}; // Pins connected to R1, R2, R3, R4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String currentlyInputtedString = "";

/*RGB LED SETUP*/
const int pinR = 18;
const int pinG = 19;
const int pinB = 20;

/*ACROSS LOOP VARIABLES*/
const unsigned int passcodeLength = 4;
const long interval = 10000;
bool isCountingStarted = false;
unsigned long currentMillis;
unsigned long beginMillis;
unsigned long endMillis;
int currentLedR = 255;
int currentLedG = 255;
int currentLedB = 255;
unsigned long pendingAlertLedChangeMilliTimestamp = 0;
unsigned long alertLedChangeMilliTimestamp = 0;
// Unsigned type to remove warning comparison signed vs unsigned
const unsigned long pendingAlertLedChangeInterval = 1000;
const unsigned long alertLedChangeInterval = 500;

GuardState guardState;

void setup()
{
  /*LCD SETUP*/
  lcd.begin(16, 2);
  /*KEYPAD SETUP*/
  Serial.begin(9600);
  /*RGB SETUP*/
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
}

void loop()
{
  /*Motion Sensor Status*/
  currentMotionSensorStatus = digitalRead(motionSensorInputPin);

  /* Activate first */
  if (guardState.isArmed() == false)
  {
    update_rgb_led(pinR, pinG, pinB, currentLedR, currentLedG, currentLedB, 255, 255, 0);
    guardState.activate(lcd, currentDisplayString, keypad, currentlyInputtedString, passcodeLength);
  }
  // Dectection Mode
  /*If reset is pressed & after activation*/
  else if (guardState.isAlertTriggered() == false && guardState.isArmed() && currentMotionSensorStatus == LOW) /*guardState.isArmed() == true for clarity*/
  {
    update_display(lcd, currentDisplayString, get_text_in_middle_row("Detection Mode") + get_text_in_middle_row("is on"));
    update_rgb_led(pinR, pinG, pinB, currentLedR, currentLedG, currentLedB, 0, 255, 0);
    // NOT Allowed to disable while no motion because it it not realistic
  }
  else if (currentMotionSensorStatus == HIGH || (guardState.isArmed() == true && guardState.isAlertTriggered() == true)) /*guardState.isArmed() == true for clarity*/
  {
    // Trigger the Alert
    if (currentMotionSensorStatus == HIGH && guardState.isArmed() == true && guardState.isAlertTriggered() == false) /*guardState.isArmed() == true for clarity*/
    {
      beginMillis = millis();
      currentMillis = millis();
      endMillis = beginMillis + interval;
      isCountingStarted = true;
      guardState.triggerAlert();
    }
    /*Start counting*/
    if (currentMillis <= endMillis && currentMillis >= beginMillis && isCountingStarted == true)
    {
      update_led_rgb_without_delay(pendingAlertLedChangeMilliTimestamp, pendingAlertLedChangeInterval, pinR, pinG, pinB, currentLedR, currentLedG, currentLedB, 255, 0, 255, 0, 0, 204);
      guardState.disable(lcd, currentDisplayString, keypad, currentlyInputtedString, passcodeLength, String("Motion/") + String(long((endMillis - currentMillis) / 1000)) + String("s left"));
      currentMillis = millis();

      // Prevent reArmed but counting is still in range
      if (guardState.isAlertTriggered() == false)
      {
        isCountingStarted = false;
      }
    }
    else
    {
      isCountingStarted = false;
    }
    // Run out of time
    if (guardState.isAlertTriggered() == true && isCountingStarted == false)
    {
      update_led_rgb_without_delay(alertLedChangeMilliTimestamp, alertLedChangeInterval, pinR, pinG, pinB, currentLedR, currentLedG, currentLedB, 255, 0, 0, 255, 255, 255);
      /*Alert Overhere*/
      guardState.disable(lcd, currentDisplayString, keypad, currentlyInputtedString, passcodeLength, String("INTRUDER ALERT!"));
    }
  }
}