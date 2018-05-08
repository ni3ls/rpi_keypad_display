#include "ctest.h"
#include <iostream>
#include "io.h"
#include <unistd.h>
using namespace std;

IO piT;
void Test::sTest() {
//    aT();
    piT.displayLcd("Test", 0);
    sleep(1);
}

void Test::aT() {
    cout << "Test" << endl;
    piT.displayLcd("Test", 0);
    sleep(1);
}
