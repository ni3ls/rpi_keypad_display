#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23x0817.h>
#include <lcd.h>
#include "io.h"

int lcd;
int fd;

void GPIOInit() {
    wiringPiSetup();
    lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    fd = wiringPiI2CSetupInterface("/dev/i2c-0", 0x20);
    // A0-A7 output
    wiringPiI2CWriteReg8(fd, MCP23x17_IODIRA, 0x00);

    // Enable B0,B1 Pullup replace 0x01 with 0xFF for all PortB
    wiringPiI2CWriteReg8 (fd, MCP23x17_GPPUB, B0 + B1) ;
    // B0,B1 input
    wiringPiI2CWriteReg8 (fd, MCP23x17_IODIRB, B0 + B1) ;
}

void displayLcd(string s, int row) {
    lcdClear(lcd);
    int column = ((16 - s.length()) / 2);
    lcdPosition(lcd, column, row);
    lcdPrintf(lcd, "%s", s.c_str());
}

void piBlink() {
    while(true) {
        piOutPin(HIGH);
        delay(500);
        piOutPin(LOW);
        delay(500);
    }
}

// HIGH or LOW
void piOutPin(int state) {
    pinMode (LED, OUTPUT);
    digitalWrite(LED, state);
}

void mcpOutPin(int pin) {
    wiringPiI2CWriteReg8(fd, MCP23x17_OLATA, pin);
}

int mcpReadPin() {
    return wiringPiI2CReadReg8 (fd, MCP23x17_GPIOB);
}

