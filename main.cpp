#include <iostream>
#include <string>
#include <unistd.h>
#include "io.h"
#include "keypad.h"
using namespace std;

int main() {

    IO pi_io;
    pi_io.displayLcd("Welcome!", 0);

    Numpad numP;
//    numP.setHiddenChar("*", true); // To set hidden char
    numP.start(); // Start timer thread
    numP.startDetectPin(); // Start dTimer thread

    while(true) {
        numP.readNumpad();
        sleep(0.5);
    }


    return 0;
}
