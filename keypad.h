#ifndef KEYPAD_H
#define KEYPAD_H

// /dev/input : linux/input.h
#define KEY_PAD "/dev/input/event0"

#include <thread>
#include <linux/input.h>
#include <string>

class Keypad {
    public:
        Keypad(int interval);
        void timerStop();
        void timerStart();
        void startKeyTh();
        void readKeypad();
        void sethideCode(bool isHidden);
        void sethiddenChar(std::string s);

    private:
        std::thread _tkeypad;
        int _interval;
        int _time;
        bool _isEnabled;
        bool _isHidden;
        void timerKeyTh();
        void reset();
        void keyTimeout();
        void attemptTimeout();
        int _input;
        int _state;
        struct input_event ev;
        std::string key_val;
        std::string hidden_val;
        std::string hidden_ch;
        int _attempts;
};

#endif
