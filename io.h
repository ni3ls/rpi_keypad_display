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
using namespace std;

class IO {
    public:
        void welcomeMsg();
        void displayLcd(string s, int row);
        void piBlink();
        void piLed(int state);
        void setMcpAx(int pin, int state);
        void mcpReset();
        int getMcpBx();
        IO();
        ~IO();

    private:
        int fd;
        int lcd;
        int column;
        int mcp_pin;
};

#endif
