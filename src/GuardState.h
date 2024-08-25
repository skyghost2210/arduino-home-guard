#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Keypad.h>
#include "utility.h"

#ifndef GUARDSTATE_H
#define GUARDSTATE_H

class GuardState
{
public:
    GuardState(); // Initialize == def __init__
    void activate(LiquidCrystal &lcd, String &currentDisplayText, Keypad &keypad, String &currentlyInputtedString, unsigned int passcodeLength);
    void disable(LiquidCrystal &lcd, String &currentDisplayText, Keypad &keypad, String &currentlyInputtedString, unsigned int passcodeLength, String displayText);
    void triggerAlert();
    bool isArmed();
    bool isAlertTriggered();

private:
    bool _armed;
    bool _alertTriggered;
    String _passcode;
};

#endif /* GUARDSTATE_H */