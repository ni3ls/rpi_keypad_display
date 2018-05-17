#include "drawer.h"
#include <thread>
#include "io.h"
#include <unistd.h>
#include <wiringPi.h>
#include <iostream>
using namespace std;

enum events {
    IS_CLOSE = 0,
    IS_OPEN,
    IS_ENABLED,
    IS_DISABLED
};

IO dr_io;

Drawer::Drawer() {
    dr_io.mcpReset();
}

void Drawer::startDrawerTh() {
    _tdrawer = thread(&Drawer::timerDrawerTh, this);
}

void Drawer::timerDrawerTh() {
    while(true) {
        usleep(500);

        switch(_drawerNum) {
            case A0: dr_io.setMcpAx(A0, HIGH); _pinA0 = HIGH; break;
            case A1: dr_io.setMcpAx(A1, HIGH); _pinA1 = HIGH; break;
        }
        _drawerNum = 0;

        switch(dr_io.getMcpBx()) {
            case 2: if(_pinA0 == HIGH) { dr_io.setMcpAx(A0, LOW); }
                 _pinA0 = LOW; break;
            case 1: if(_pinA1 == HIGH) { dr_io.setMcpAx(A1, LOW); }
                 _pinA1 = LOW; break;
        }
    }
}

void Drawer::openDrawer(int drawerNum) {
    _drawerNum = drawerNum;
}
