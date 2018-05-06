#ifndef KEYPAD_H
#define KEYPAD_H

// /dev/input : linux/input.h
#define KEYPAD "/dev/input/event0"

#include <string>
#include <linux/input.h>
using namespace std;

class NUMPAD {
    public:
        void readNumpad();
        int getNumpad();
        NUMPAD();

    private:
        string key_value;
        int input;
        int ch;
        input_event ev;
};

#endif
