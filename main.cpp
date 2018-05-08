#include <iostream>
#include <string>
#include "io.h"
#include "keypad.h"
#include <unistd.h>
using namespace std;

int main() {

    IO pi_io;
    pi_io.displayLcd("Welcome!", 0);

    Numpad numP;
//    numP.setHiddenChar("*");
    numP.start();

    while(true) {
        numP.readNumpad();
    }


    return 0;
}
