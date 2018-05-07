#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23x0817.h>
#include <lcd.h>
#include "io.h"

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
    lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    fd = wiringPiI2CSetupInterface("/dev/i2c-0", 0x20);
    // A0-A7 output
    wiringPiI2CWriteReg8(fd, MCP23x17_IODIRA, 0x00);
}

IO::~IO() {
    lcdClear(lcd);
    mcpReset();
    piLed(LOW);
}

void IO::cursorBlink(int state) {
    switch(state) {
        case 0:
              lcdCursor(lcd, 0);
              lcdCursorBlink(lcd, 0);
              break;
        case 1:
              lcdCursor(lcd, 1);
              lcdCursorBlink(lcd, 1);
              break;
    }
}

void IO::displayLcd(string s, int row) {
    lcdClear(lcd);
//    lcdCursor(lcd, 1);
//    lcdCursorBlink(lcd, 1);
    column = ((16 - s.length()) / 2);
    lcdPosition(lcd, column, row);
    lcdPrintf(lcd, "%s", s.c_str());
}

void IO::piBlink() {
    piLed(HIGH);
    delay(500);
    piLed(LOW);
    delay(500);
}

void IO::piLed(int state) {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, state);
}

void IO::setMcpAx(int pin, int state) {
    switch(state) {
        case 0: mcp_pin -= pin; break;
        case 1: mcp_pin += pin; break;
    }
    if(mcp_pin >= 0)
        wiringPiI2CWriteReg8(fd, MCP23x17_OLATA, mcp_pin);
}

void IO::mcpReset() {
    wiringPiI2CWriteReg8(fd, MCP23x17_OLATA, 0);
}

int IO::getMcpBx() {
    return wiringPiI2CReadReg8 (fd, MCP23x17_GPIOB);
}
