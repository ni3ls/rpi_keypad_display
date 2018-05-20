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

class IO {
    public:
        IO();
        void displayLcd(const std::string &s, const int &row);
        void cursorBlink(bool blink);
        void piBlink();
        void piLed(const bool &isHigh);
        void setMcpAx(const int &pin, const bool &isHigh);
        void mcpReset();
        int getMcpBx();
        int getMcpActivePin();

    private:
        int _fd;
        int _lcd;
        int _column;
        int mcp_pin;
};

#endif
