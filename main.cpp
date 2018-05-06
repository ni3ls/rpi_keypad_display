#include <iostream>
#include <unistd.h>
#include "io.h"
#include "keypad.h"
#include <thread>
using namespace std;


int main() {
    cout << A0 << endl;
    cout << B7 << endl;

    NUMPAD numpad;
    while(true) {
    numpad.readNumpad();
    numpad.getNumpad();
    }
    return 0;
}
