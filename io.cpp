#include <string>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23x0817.h>
#include <lcd.h>
#include "io.h"
#include <iostream>
using namespace std;

// 2x16 LCD
//USE WIRINGPI PIN NUMBERS                                Board Pin
#define LCD_RS  13               //Register select pi     21
#define LCD_E   12               //Enable Pin             19
#define LCD_D4  11               //Data pin 4             26
#define LCD_D5  10               //Data pin 5             24
#define LCD_D6  6                //Data pin 6             22
#define LCD_D7  5                //Data pin 7             18

//           WiringPi     Board
// Led Pin = 29           40
#define LED  29

IO::IO() {
    wiringPiSetup();
    _lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    _fd = wiringPiI2CSetupInterface("/dev/i2c-0", 0x20);
    // A0-A7 output
    wiringPiI2CWriteReg8(_fd, MCP23x17_IODIRA, 0x00);

    mcp_pin = 0;
}

void IO::piBlink() {
    piLed(HIGH);
    delay(1000);
    piLed(LOW);
    delay(1000);
    piLed(HIGH);
    delay(1000);
    piLed(LOW);
    delay(1000);
}

void IO::piLed(const bool &isHigh) {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, isHigh);
}

void IO::setMcpAx(const int &pin, const bool &isHigh) {
    switch(isHigh) {
        case LOW: mcp_pin -= pin; break;
        case HIGH: mcp_pin += pin; break;
    }
    if(mcp_pin >= 0)
        wiringPiI2CWriteReg8(_fd, MCP23x17_OLATA, mcp_pin);
    cout << mcp_pin << endl;
}

void IO::mcpReset() {
    wiringPiI2CWriteReg8(_fd, MCP23x17_OLATA, 0);
}

int IO::getMcpBx() {
    return wiringPiI2CReadReg8(_fd, MCP23x17_GPIOB);
}

void IO::cursorBlink(bool blink) {
    switch(blink) {
        case false:
              lcdCursor(_lcd, 0);
              lcdCursorBlink(_lcd, 0);
              break;
        case true:
              lcdCursor(_lcd, 1);
              lcdCursorBlink(_lcd, 1);
              break;
    }
}

void IO::displayLcd(const string &s, const int &row) {
    lcdClear(_lcd);
    _column = ((16 - s.length()) / 2);
    lcdPosition(_lcd, _column, row);
    lcdPrintf(_lcd, "%s", s.c_str());
}

void IO::setxVal() {
    _xval = 1;
}
