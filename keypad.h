#ifndef KEYPAD_H
#define KEYPAD_H

// /dev/input : linux/input.h
#define KEYPAD "/dev/input/event0"

#include <string>
#include <linux/input.h>
#include <thread>
#include <wiringPi.h>
#include <iostream>
using namespace std;

class Numpad {
    public:
        Numpad();
        ~Numpad() {}
        void readNumpad();
        void setHiddenChar(const bool &hide);
        void enterPinAgain();
        void start() { tmr = thread(&Numpad::xTimer, this); }
        void startDetectPin() { detect = thread(&Numpad::dTimer, this); }

    private:
        string key_value;
        string hidden_value;
        string hidden_char;
        int _input;
        int _ts;
        int _xpin;
        int _blockNumpad;
        bool _stopTimer;
        bool _isHidden;
        bool _isEnterKey;
        struct input_event ev;
        void setIdle();
        void timeOut();
        void A0pin();
        void xTimer() {
            while(true) {
                while(!_stopTimer) {
                    _ts += 1;;
                    delay(1000);
                    cout << _ts << endl;
                    if(_ts == 5) {
                        _ts = 1;
                        _stopTimer = true;
                        timeOut();
                    }
                }
            }
        }
        thread tmr;
        void dTimer() {
            while(true) {
                A0pin();
            }
        }
        thread detect;
};
#endif
