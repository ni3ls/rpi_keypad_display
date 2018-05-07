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
        void setHidden(const char* s, int hide);
        string getNumpad();
        string getHiddenNumpad();
        int isEnterKey();
        NUMPAD();

    private:
        string key_value;
        string hidden_value;
        string hidden_char;
        int hide;
        int ch;
        int input;
        struct input_event ev;
};

#endif
