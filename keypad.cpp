#include "keypad.h"

int keypadValue(int key_code) {
    int ch;
    switch(key_code) {
        case 2: ch = 1; break;
        case 3: ch = 2; break;
        case 4: ch = 3; break;
        case 5: ch = 4; break;
        case 6: ch = 5; break;
        case 7: ch = 6; break;
        case 8: ch = 7; break;
        case 9: ch = 8; break;
        case 10: ch = 9; break;
        case 11: ch = 0; break;
        }
    return ch;
}
