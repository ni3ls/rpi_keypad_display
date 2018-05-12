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
        void startDrawerX(int pin) {
            _pin = pin;
            pi_io.setMcpAx(pin, HIGH);
        }
        void startDrawerA0Timer() { drawerThA0 = thread(&DrawerTh::dThA0, this); }
        void startDrawerA1Timer() { drawerThA1 = thread(&DrawerTh::dThA1, this); }

    private:
        thread drawerThA0;
        thread drawerThA1;
        int _pin;
        bool _isLeft;
        int dTimer;
        void dThA0() {
            while(true) {
                switch(_pin) {
                    case 1:
                         _isLeft = false;
                         while(!_isLeft) {
                             dTimer += 1;
                             if(pi_io.getMcpBx() == 2 && _pin == 1) {
                                 _isLeft = true;
                                 pi_io.setMcpAx(A0, LOW);
                                 _pin = 0;
                                 dTimer = 0;
                             }
                             cout << "Test thread" << dTimer << endl;
                             cout << "mcp: " << pi_io.getMcpBx() << endl;
                             delay(250);
                             if(dTimer == 20) {
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
        void dThA1() {
            while(true) {
                switch(_pin) {
                    case 2:
                         _isLeft = false;
                         while(!_isLeft) {
                             dTimer += 1;
                             if(pi_io.getMcpBx() == 1 && _pin == 2) {
                                 _isLeft = true;
                                 pi_io.setMcpAx(A1, LOW);
                                 _pin = 0;
                                 dTimer = 0;
                             }
                             cout << "Test thread" << dTimer << endl;
                             cout << "mcp: " << pi_io.getMcpBx() << endl;
                             delay(250);
                             if(dTimer == 20) {
                                 while(!_isLeft) {
                                     if(pi_io.getMcpBx() == 1 && _pin == 2) {
                                         _isLeft = true;
                                         pi_io.setMcpAx(A1, LOW);
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
