#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Keypad.h>
#include <EEPROM.h>
#include "GuardState.h"
#include "utility.h"

#define EEPROM_ADDR_ARMED 0
#define EEPROM_ADDR_ALERTTRIGGERED 1
#define EEPROM_ADDR_PASSCODE 2

GuardState::GuardState()
{
    // this->_armed = false;
    // this->_alertTriggered = false;
    // this->_passcode = "";
    this->_armed = EEPROM.read(EEPROM_ADDR_ARMED) == 1;
    this->_alertTriggered = EEPROM.read(EEPROM_ADDR_ALERTTRIGGERED) == 1;
    this->_passcode = get_text_EEPROM(EEPROM_ADDR_PASSCODE);
}

void GuardState::activate(LiquidCrystal &lcd, String &currentDisplayText, Keypad &keypad, String &currentlyInputtedString, unsigned int passcodeLength)
{
    String passcode = get_inputted_passcode(lcd, currentDisplayText, keypad, currentlyInputtedString, 4, "Detection off");
    if (passcode.length() == passcodeLength)
    {
        this->_passcode = passcode;
        this->_armed = true;
        update_display(lcd, currentDisplayText, get_text_in_middle_row("Activated") + get_text_in_middle_row("5s to leave"));
        EEPROM.write(EEPROM_ADDR_ARMED, this->_armed ? 1 : 0);
        save_text_EEPROM(this->_passcode, EEPROM_ADDR_PASSCODE);
        delay(5000);
    }
}

/*Two types of disable: When motion detected and when normal*/
void GuardState::disable(LiquidCrystal &lcd, String &currentDisplayText, Keypad &keypad, String &currentlyInputtedString, unsigned int passcodeLength, String displayText)
{
    String passcode = get_inputted_passcode(lcd, currentDisplayText, keypad, currentlyInputtedString, 4, displayText);
    if (passcode == this->_passcode)
    {
        this->_armed = false;
        this->_alertTriggered = false;
        this->_passcode = "";
        EEPROM.write(EEPROM_ADDR_ARMED, this->_armed ? 1 : 0);
        save_text_EEPROM(this->_passcode, EEPROM_ADDR_PASSCODE);
        EEPROM.write(EEPROM_ADDR_ALERTTRIGGERED, this->_alertTriggered ? 1 : 0);
    }
}

void GuardState::triggerAlert()
{

    this->_alertTriggered = true;
    EEPROM.write(EEPROM_ADDR_ALERTTRIGGERED, this->_alertTriggered ? 1 : 0);
}

bool GuardState::isArmed()
{
    if (this->_armed == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool GuardState::isAlertTriggered()
{
    if (this->_alertTriggered == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}