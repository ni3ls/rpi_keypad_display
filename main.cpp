#include <iostream>
#include <unistd.h>
#include "io.h"
#include "keypad.h"
#include <thread>
using namespace std;


int main() {
    cout << A0 << endl;
    cout << B7 << endl;
    IO pi3_io;


    NUMPAD numpad;
        pi3_io.displayLcd("Welcome!", 0);
    while(true) {
        numpad.setHidden("*", 1);
        numpad.readNumpad();
        pi3_io.displayLcd(numpad.getHiddenNumpad(), 0);

        if(numpad.isEnterKey() == 1)
            cout << numpad.getNumpad() << endl;
    }
    return 0;
}
