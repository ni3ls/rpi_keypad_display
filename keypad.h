#ifndef KEYPAD_H
#define KEYPAD_H

// /dev/input : linux/input.h
#define KEYPAD "/dev/input/event0"

#include <string>
#include <linux/input.h>
#include <thread>
#include <iostream>
#include <unistd.h>
using namespace std;

class Numpad {
    public:
        void readNumpad();
        void setHiddenChar(string x);
        bool isEnter();
        Numpad();
        void start() { tmr = thread(&Numpad::xTimer, this); }

    private:
        bool ent;
        string key_value;
        string hidden_value;
        string hidden_char;
        int input;
        struct input_event ev;
        void xTimer() const {
            int x = 0;
            while(true) {
                ++x;
                cout << x << endl;
                sleep(1);
            }
        }
        thread tmr;
};
#endif
