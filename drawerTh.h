#ifndef DRAWER_TH_H
#define DRAWER_TH_H

#include <thread>
#include <iostream>
#include <unistd.h>
#include <string>
#include "io.h"
#include <wiringPi.h>
using namespace std;

IO pi_io;

class DrawerTh {
    public:
        DrawerTh() { _isLeft = false; }
        ~DrawerTh() {}
        void startDrawerThread(int pin) {
            _pin = pin;
            pi_io.setMcpAx(pin, HIGH);
        }
        void startDrawerTimer() { drawerTh = thread(&DrawerTh::dTh, this); }

    private:
        thread drawerTh;
        int _pin;
        bool _isLeft;
        int dTimer;
        void dTh() {
            while(true) {
                switch(_pin) {
                    case 1:
                         _isLeft = false;
                         while(!_isLeft) {
                             dTimer += 1;
                             if(pi_io.getMcpBx() == 2) {
                                 _isLeft = true;
                                 if(_pin == 1 && _isLeft == 1) {
                                     pi_io.setMcpAx(A0, LOW);
                                     _pin = 0;
                                 }
                                 cout << "isLeft: " << _isLeft << endl;
                                 dTimer = 0;
                             }
                             cout << "Test thread" << dTimer << endl;
                             cout << "mcp: " << pi_io.getMcpBx() << endl;
                             delay(500);
                             if(dTimer == 10) {
                                 while(!_isLeft) {
                                     if(pi_io.getMcpBx() == 2 && _pin == 1) {
                                         _isLeft = true;
                                         pi_io.setMcpAx(A0, LOW);
                                         _pin = 0;
                                         dTimer = 0;
                                     }
                                     pi_io.piBlink();
                                 }
                                 pi_io.piLed(LOW);
                             }
                         }
                         break;
                }
            }
        }

};

#endif
