#include "keypad.h"
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;

NUMPAD::NUMPAD() {
    char data[] = KEYPAD;
    input = open(data, O_RDONLY);
}

void NUMPAD::setHidden(const char* s, int h) {
    hide = h;
    hidden_char = s;
}

void NUMPAD::readNumpad() {
    read(input, &ev, sizeof(ev));

    if(ev.type == 1 && ev.value == 1) {
        // Enter key
        ch = (ev.code == 28) ? 1 : 0;

        if(ev.code >= 2 && ev.code <= 11) {
            if(ev.code == 11)
                key_value += "0";
            else
                key_value += to_string(ev.code - 1);

            if(hide == 1)
                hidden_value += hidden_char;
        }

        // DEL or BACKSPACE
        if(ev.code == 111 || ev.code == 14) {
            if(key_value.length() > 0) {
                key_value.resize(key_value.length() - 1);

               if(hide == 1)
                   hidden_value.resize(hidden_value.length() - 1);
            }
        }
    }
}

int NUMPAD::isEnterKey() {
    return ch;
}

string NUMPAD::getHiddenNumpad() {
    return hidden_value;
}

string NUMPAD::getNumpad() {
    return key_value;
}
