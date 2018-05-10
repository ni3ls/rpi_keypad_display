#include "keypad.h"
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstdio>
#include "io.h"

#include <iostream>
using namespace std;

IO piIO;

Numpad::Numpad() {
    char data[] = KEYPAD;
    _input = open(data, O_RDONLY);
    _stopTimer = true;
    _ts = 1;
    _blockNumpad = 0;
    _isHidden = false;
    hidden_char = "*";
}

void Numpad::readNumpad() {
    if(_blockNumpad == 0) {
        read(_input, &ev, sizeof(ev));

        if(ev.type == 1 && ev.value == 1) {
            _stopTimer = true;
            _ts = 1;
            switch(ev.code) {
                case 11:
                    key_value += "0";
                    hidden_value += hidden_char;
                    break;
                case 2 ... 10: // 1-9
                    key_value += to_string(ev.code -1);
                    hidden_value += hidden_char;
                    break;
                case 111: case 14: // Del Backspace
                    if(key_value.length() > 0) {
                        key_value.resize(key_value.length() - 1);
                        hidden_value.resize(hidden_value.length() - 1);
                    }
                    break;
            }
            _stopTimer = false;

            if(ev.code == 28) {
                _stopTimer = true;
                _ts = 1;
                _isEnterKey = true;
            }

            (key_value.length() == 0) ? piIO.cursorBlink(1) 
            : piIO.cursorBlink(0);
            piIO.displayLcd((_isHidden == false) ? key_value : hidden_value , 0);
        }
    }
}

void Numpad::timeOut() {
    _blockNumpad = 1;
    piIO.cursorBlink(0);
    piIO.displayLcd("Sorry too slow", 0);
    delay(2000);
    setIdle();
}

void Numpad::setIdle() {
    piIO.displayLcd("Welcome!", 0);
    _blockNumpad = 0;
    key_value = "";
    hidden_value = "";
}

void Numpad::setHiddenChar(const string &x, const bool &hide) {
    hidden_char = x;
    _isHidden = hide;
}
