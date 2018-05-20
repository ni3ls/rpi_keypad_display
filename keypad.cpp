#include "keypad.h"
#include <unistd.h>
#include <fcntl.h>
#include "io.h"
#include <string>
#include "verifyPCode.h"
#include "drawer.h"
using namespace std;

enum states {
    IS_RESET = 0,
    IS_READING,
    IS_ENTER
};

IO pi_io;
VerifyPCode vcode;
Drawer drawer;

Keypad::Keypad(int interval) {
    _interval = interval;
    _time = 0;
    _isEnabled = false;
    char data[] = KEY_PAD;
    _input = open(data, O_RDONLY);
    _state = IS_RESET;
    hidden_ch = "*";
    _isHidden = false;
    _attempts = 0;
    drawer.startDrawerTh();
    pi_io.mcpReset();
}

void Keypad::readKeypad() {
    switch(_state) {
        case IS_RESET: reset(); _state = IS_READING; break;
        case IS_READING:
             read(_input, &ev, sizeof(ev));
            if(ev.type == 1 && ev.value == 1) {
                timerStop(); _time = 0;
                switch(ev.code) {
                    case 11: key_val += "0"; hidden_val += hidden_ch;
                        _state = IS_READING; break;
                    case 2 ... 10: // 1-9
                        key_val += to_string(ev.code -1); hidden_val += hidden_ch;
                        _state = IS_READING; break;
                    case 111: case 14: // Del Backspace
                        if(key_val.length() > 0) {
                            key_val.resize(key_val.length() - 1);
                            hidden_val.resize(hidden_val.length() - 1);
                        }
                        _state = IS_READING; break;
                    case 28: _state = IS_ENTER; break;
                }
                (key_val.length() == 0) ? pi_io.cursorBlink(1)
                    : pi_io.cursorBlink(0);
                pi_io.displayLcd((_isHidden == false) ? key_val : hidden_val, 0);
                (_state == IS_ENTER) ? timerStop() : timerStart();
            }
            break;
        case IS_ENTER:
            if(key_val.length() > 0) {
                if(vcode.verifyPinCode(key_val) == "PIN ERROR") {
                    _attempts++;
                    switch(_attempts) {
                        case 3: attemptTimeout(); _attempts = 0;
                             _state = IS_RESET; break;
                        case 1 ... 2: sleep(2); key_val = "";
                             hidden_val = ""; pi_io.cursorBlink(1);
                             pi_io.displayLcd("", 0); timerStart();
                             _state = IS_READING; break;
                    }
                }
                else {
                    _attempts = 0;
                    switch(stoi(key_val)) {
                        case 1234: drawer.openDrawer(A0); break;
                        case 5678: drawer.openDrawer(A1); break;
                    }
                    sleep(2); _state = IS_RESET;
                }
            }
            break;
    }

}

void Keypad::attemptTimeout() {
    string atimeout = "Timeout " + to_string(_interval)  +  " secs";
    pi_io.displayLcd("To many attempts" + atimeout, 0);
    sleep(_interval); reset();
}

void Keypad::keyTimeout() {
    pi_io.cursorBlink(0);
    pi_io.displayLcd("Sorry too slow", 0);
    sleep(2); reset();
}

void Keypad::reset() {
    key_val = "";
    hidden_val = "";
    pi_io.displayLcd("Welcome!", 0);
}

void Keypad::startKeyTh() {
    _tkeypad = thread(&Keypad::timerKeyTh, this);
}

void Keypad::timerStart() {
    _isEnabled = true;
}

void Keypad::timerStop() {
    _isEnabled = false;
}

void Keypad::sethideCode(bool isHidden) {
    _isHidden = isHidden;
}

void Keypad::sethiddenChar(string s) {
    hidden_ch = s;
}

void Keypad::timerKeyTh() {
    while(true) {
        switch(_isEnabled) {
            case 0: usleep(500); _time = 0; break;
            case 1: _time++;
                 if(_time == _interval) {
                     _isEnabled = false; _time = 0;
                     keyTimeout();
                 }
                 else
                     sleep(1);
                 break;
        }
    }
}
