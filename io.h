#ifndef DISPLAY_IO_H
#define DISPLAY_IO_H

// I2C mcp23017
#define A0 1
#define A1 2
#define A2 4
#define A3 8
#define A4 16
#define A5 32
#define A6 64
#define A7 128

#define B0 1
#define B1 2
#define B2 4
#define B3 8
#define B4 16
#define B5 32
#define B6 64
#define B7 128

#include <string>
#include <thread>
#include <iostream>
#include <unistd.h>
using namespace std;

class IO {
    public:
        IO();
        ~IO() {}
        void displayLcd(const string &s, const int &row);
        void cursorBlink(bool blink);
        void piBlink();
        void piLed(const bool &isHigh);
        void setMcpAx(const int &pin, const bool &isHigh);
        void mcpReset();
        int getMcpBx();
        void start() { tmr2 = thread(&IO::mcpTimer, this); }
        void setxVal();

    private:
        int _fd;
        int _lcd;
        int _column;
        int mcp_pin;
        int _xval;
        void mcpTimer() {
            while(true) {
                if(_xval == 1) {
                    piBlink();
                    _xval = 0;
                }
            }
        }
        thread tmr2;
};

#endif
