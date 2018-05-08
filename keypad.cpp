#include "keypad.h"
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstdio>
#include "io.h"
using namespace std;

IO piNum;

Numpad::Numpad() {
    char data[] = KEYPAD;
    input = open(data, O_RDONLY);
}

void Numpad::setHiddenChar(string x) {
    hidden_char = x;
}

void Numpad::readNumpad() {
    read(input, &ev, sizeof(ev));

    ent = (ev.code == 28) ? true : false;

    if(ev.type == 1 && ev.value == 1) {
        // Nums 0-9
        if(ev.code >= 2 && ev.code <= 11) {
            if(ev.code == 11) {
                key_value += "0";
            }
            else
                key_value += to_string(ev.code - 1);
            hidden_value += hidden_char;
        }

        // DEL or BACKSPACE
        if(ev.code == 111 || ev.code == 14) {
            if(key_value.length() > 0)
                key_value.resize(key_value.length() - 1);
                if(hidden_char != "")
                    hidden_value.resize(hidden_value.length() - 1);
        }

        (key_value.length() == 0) ? piNum.cursorBlink(1) : piNum.cursorBlink(0);
        piNum.displayLcd((hidden_char == "") ? key_value : hidden_value , 0);
    }
}

bool Numpad::isEnter() {
    return ent;
}
