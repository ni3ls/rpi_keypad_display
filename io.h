#ifndef DISPLAY_IO_H
#define DISPLAY_IO_H

// 2x16 LCD
//USE WIRINGPI PIN NUMBERS                                Board Pin
#define LCD_RS  13               //Register select pi     21
#define LCD_E   12               //Enable Pin             19
#define LCD_D4  11               //Data pin 4             26
#define LCD_D5  10               //Data pin 5             24
#define LCD_D6  6                //Data pin 6             22
#define LCD_D7  5                //Data pin 7             18

#include <string>
using namespace std;

void displayLcd(string s, int row);
void GPIOInit();

#define HIGH 1
#define LOW  0
//           WiringPi     Board
// Led Pin = 29           40
#define LED  29

void piOutPin(int state);
void piBlink();

// I2C mcp23017
#define A0 0x01
#define A1 0x02
#define A2 0x04
#define A3 0x08
#define A4 0x10
#define A5 0x20
#define A6 0x40
#define A7 0x80

#define B0 0x01
#define B1 0x02
#define B2 0x04
#define B3 0x08
#define B4 0x10
#define B5 0x20
#define B6 0x30
#define B7 0x40

void mcpOutPin(int pin);
int mcpReadPin();

#endif
