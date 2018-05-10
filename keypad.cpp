#include "keypad.h"
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstdio>
#include "io.h"
#include "verifyPCode.h"
using namespace std;

IO piIO;
VerifyPCode vcode;

Numpad::Numpad() {
    char data[] = KEYPAD;
    _input = open(data, O_RDONLY);
    _stopTimer = true;
    _ts = 1;
    _blockNumpad = 0;
    _isHidden = false;
    hidden_char = "*";
    piIO.mcpReset();
    piIO.piLed(LOW);
    piIO.start();
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

            (key_value.length() == 0) ? piIO.cursorBlink(1) 
            : piIO.cursorBlink(0);
            piIO.displayLcd((_isHidden == false) ? key_value : hidden_value , 0);

            if(ev.code == 28 && key_value.length() > 0) {
                _stopTimer = true;
                _ts = 1;
                _isEnterKey = true;
                // To-do  check PCode
                if(vcode.verifyPCode(key_value) == "PIN ERROR") {
                        delay(1500);
                        key_value = "";
                        piIO.cursorBlink(1);
                        piIO.displayLcd("", 0);
                        _stopTimer = false;
                }
                else {
                    piIO.setMcpAx(A0, HIGH);
                    delay(1500);
                    setIdle();
                    piIO.cursorBlink(0);
                    piIO.setxVal();
                    _xpin = 1;
                }
            }
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

// testing
void Numpad::A0pin() {
    if(piIO.getMcpBx() == 1 && _xpin == 1) {
        piIO.setMcpAx(A0, LOW);
        _xpin = 0;
    }
}
