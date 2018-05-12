#include "keypad.h"
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstdio>
#include "io.h"
#include "verifyPCode.h"
#include "config.h"
#include "drawerTh.h"
using namespace std;

Config conf;
IO piIO;
VerifyPCode vcode;
DrawerTh drawer;

Numpad::Numpad() {
    char data[] = KEYPAD;
    _input = open(data, O_RDONLY);
    _stopTimer = true;
    _blockNumpad = 0;
    _attempts = 0;
    _isHidden = false;
    hidden_char = "*"; // default
    piIO.mcpReset();
    piIO.piLed(LOW);
//    piIO.start();
    loadConfig(conf);
    _tsConf = conf.key_pause;
    drawer.startDrawerA0Timer();
    drawer.startDrawerA1Timer();
}

void Numpad::readNumpad() {
    if(_blockNumpad == 0) {
        read(_input, &ev, sizeof(ev));

        if(ev.type == 1 && ev.value == 1) {
            _stopTimer = true;
            _ts = 0;
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
                _ts = 0;
                _isEnterKey = true;
                // To-do  check PCode
                if(vcode.verifyPCode(key_value) == "PIN ERROR") {
                        ++_attempts;
                        switch(_attempts) {
                            case 3: attemptTimeout(); _attempts = 0; break;
                            case 1 ... 2:
                                delay(1500);
                                key_value = "";
                                hidden_value = "";
                                piIO.cursorBlink(1);
                                piIO.displayLcd("", 0);
                                _stopTimer = false;
                                break;
                       }
                }
                else {
                    _attempts = 0;
                    switch(stoi(key_value)) {
                        case 1234: //piIO.setMcpAx(A0, HIGH);
                                   drawer.startDrawerX(A0); break;
                        case 5678: //piIO.setMcpAx(A1, HIGH);
                                   drawer.startDrawerX(A1); break;
                    }

                    delay(1500);
                    setIdle();
                    piIO.cursorBlink(0);
                    piIO.setxVal();
                    _xpin = 1;
//                    drawer.startDrawerThread();
                }
            }
        }
    }
}

void Numpad::attemptTimeout() {
    string atimeout = "Timeout " + to_string(conf.retention)  +  " secs";
    piIO.displayLcd("To many attempts" + atimeout, 0);
    sleep(conf.retention);
    setIdle();
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

void Numpad::setHiddenChar(const bool &hide) {
    hidden_char = conf.hidden_char;
    _isHidden = hide;
}

// testing
void Numpad::A0pin() {
    if(piIO.getMcpBx() == 1 && _xpin == 1) {
        piIO.setMcpAx(A0, LOW);
        _xpin = 0;
    }
}
