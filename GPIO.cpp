#include <iostream>
#include <string>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23x0817.h>
#include <lcd.h>
#include "GPIO.h"
using namespace std;

int lcd;
int fd;

void GPIOInit() {
    wiringPiSetup();
    lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    fd = wiringPiI2CSetupInterface("/dev/i2c-0", 0x20);
    // A0-A7 output
    wiringPiI2CWriteReg8(fd, MCP23x17_IODIRA, 0x00);
    // B0 input
    wiringPiI2CWriteReg8 (fd, MCP23x17_IODIRB, 0x01) ;
}

void displayLcd(string s, int row) {
    lcdClear(lcd);
    int column = ((16 - s.length()) / 2);
    lcdPosition(lcd, column, row);
    lcdPrintf(lcd, "%s", s.c_str());
}

void outputPinHigh() {
    pinMode (LED, OUTPUT);
    digitalWrite(LED, HIGH);
}

void outputPinLow() {
    pinMode (LED, OUTPUT);
    digitalWrite(LED, LOW);
}

void i2cOutPinHigh() {
    wiringPiI2CWriteReg8(fd, I2C_AOUT, A0);
}

void i2cOutPinLow() {
    wiringPiI2CWriteReg8(fd, I2C_AOUT, 0x00);
}

int i2cInput() {
    return wiringPiI2CReadReg8 (fd, I2C_BIN);
}
