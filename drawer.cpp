#include "drawer.h"
#include <thread>
#include "io.h"
#include <unistd.h>
#include <wiringPi.h>
#include <chrono>
#include "config.h"
#include <iostream>
using namespace std;

enum events {
    IS_CLOSE = 0,
    IS_OPEN,
    IS_ENABLED,
    IS_DISABLED,
};

IO dr_io;

Drawer::Drawer() {
    dr_io.mcpReset();
}

void Drawer::startDrawerTh() {
    _tdrawer = thread(&Drawer::timerDrawerTh, this);
}

void Drawer::timerDrawerTh() {
    chrono::time_point<chrono::system_clock> _pinA0STime, _pinA1STime;
    Config conf;
    loadConfig(conf);

    while(true) {
        usleep(250);

        switch(_drawerNum) {
            case A0:
                 if(_pinA0 != HIGH) {
                     dr_io.setMcpAx(A0, HIGH);
                     _pinA0STime = chrono::system_clock::now();
                     _pinA0 = HIGH; break;
                 }
            case A1:
                 if(_pinA1 != HIGH) {
                     dr_io.setMcpAx(A1, HIGH);
                     _pinA1STime = chrono::system_clock::now();
                     _pinA1 = HIGH; break;
                 }
        }
        _drawerNum = 0;

        switch(dr_io.getMcpBx()) {
            case 2: if(_pinA0 == HIGH) { dr_io.setMcpAx(A0, LOW); }
                 _pinA0 = LOW; break;
            case 1: if(_pinA1 == HIGH) { dr_io.setMcpAx(A1, LOW); }
                 _pinA1 = LOW; break;
        }

        if(_pinA0 == HIGH) {
            chrono::duration<double> elapsed_seconds = 
                chrono::system_clock::now() - _pinA0STime;
            if((int)elapsed_seconds.count() == conf.drawer_open_time) {
                dr_io.piBlink();
            }

            if((int)elapsed_seconds.count() >= conf.drawer_open_err_time)
                cout << "drawer A0 opened for too long" << endl;
        }

        if(_pinA1 == HIGH) {
            chrono::duration<double> elapsed_seconds =
                chrono::system_clock::now() - _pinA1STime;
            if((int)elapsed_seconds.count() == conf.drawer_open_time) {
                dr_io.piBlink();
            }

            if((int)elapsed_seconds.count() >= conf.drawer_open_err_time)
                cout << "drawer A1 opened for too long" << endl;
        }
    }
}

void Drawer::openDrawer(int drawerNum) {
    _drawerNum = drawerNum;
}
