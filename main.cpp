#include <iostream>
#include <unistd.h>
#include "io.h"
#include "checkPassCode.h"
#include "keypad.h"
#include <wiringPi.h>

// Keypad
#include <linux/input.h>
#include <cstdio>
#include <fcntl.h>

#include <atomic>
#include <thread>
using namespace std;

// Key press Timer
atomic<bool> stopTimer;
int ts;
int key_timeout;

string pin_code;
string hidden_pin;

void setToIdle() {
    pin_code = "";
    hidden_pin = "";
    displayLcd("Welcome!",0);
}

void keyTimeout() {
    ts = 0;
    while(true) {
        while(!stopTimer) {
            ts += 1;
            delay(1000);

            if(ts == key_timeout) {
                stopTimer = true;
                displayLcd("Sorry to slow", 0);
                delay(2000);
                setToIdle();
            }
        }
    }
}

// test
int user1;
int user2;

void drawerMon() {
    while(true) {
        switch(mcpReadPin()) {
            case 0x02:
                     if(user1 == 1) {
                         if(user2 == 1)
                            mcpOutPin(A1);
                        else
                            mcpOutPin(0);
                     }
                     user1 = 0;
                     break;
            case 0x01:
                     if(user2 == 1) {
                         if(user1 ==1)
                             mcpOutPin(A0);
                         else
                             mcpOutPin(0);
                     }
                     user2 = 0;
                     break;
        }
    }
}

int main() {
    Config config;
    loadConfig(config);

    key_timeout = config.key_pause;
    stopTimer = true;

    char data[] = KEYPAD;
    int input = open(data, O_RDONLY);
    struct input_event ev;

    int attempts = 0;

    GPIOInit();
    displayLcd("Welcome!", 0);

    thread th(keyTimeout);
    thread th2(drawerMon);

    piOutPin(LOW);
    mcpOutPin(0);

    while(true) {
        read(input, &ev, sizeof(ev));

        if(ev.type == 1 && ev.value == 1) {
            stopTimer = false;
            ts = 0;

            // DEL or BACKSPACE
            if(ev.code == 111 || ev.code == 14) {
                if(pin_code.length() != 0) {
                    pin_code.resize(pin_code.length() - 1);
                    if(config.hide_char) {
                        hidden_pin.resize(hidden_pin.length() -1);
                        displayLcd(hidden_pin, 0);
                    }
                    else {
                        displayLcd(pin_code, 0);
                    }
                }
            }

            // ENTER
            else if(ev.code == 28) {
                string auth = checkPassCode(pin_code);
                if(auth == "PIN OK") {
                    displayLcd(auth, 1);
                    attempts = 0;

                    stopTimer = true;
                    ts = 0;

                    // Box open not ideal
                    // for test only
                    switch(stoi(pin_code)) {
                        case 1234: user1 = 1; break;
                        case 5678: user2 = 1; break;
                    }

                    if(user1 == 1) {
                        if(user2 == 1)
                            mcpOutPin(A1 + A0);
                        else
                            mcpOutPin(A0);
                    }

                    if(user2 == 1) {
                        if(user1 == 1)
                            mcpOutPin(A1 + A0);
                        else
                            mcpOutPin(A1);
                    }


                    delay(1000);
                    setToIdle();
                }
                else {
                    displayLcd(auth, 1);
                    ++attempts;

                    stopTimer = true;
                    ts = 0;

//                    outputPin(LOW);
//                    i2cOutPin(0);

                    delay(1000);
                    setToIdle();
                }
            }

            else {
                pin_code += to_string(keypadValue(ev.code));
                    if(config.hide_char) {
                        hidden_pin += config.hidden_char;
                        displayLcd(hidden_pin, 0);
                    }
                    else
                        displayLcd(pin_code, 0);
            }

            // Max attempts
            if(attempts == config.attempts) {
                string timeout = "Timeout " + to_string(config.retention)  +  " secs";
                displayLcd("To many attempts" + timeout, 0);
                attempts = 0;

                stopTimer = true;
                ts = 0;

                sleep(config.retention);
                displayLcd("Welcome!", 0);
            }

            // max digits uncomment to enable
            /*if (pin_code.length() == config.max_digits) {
                  string auth = checkPincode(pin_code);
                  if(auth == "PIN OK" {
                      attempts = 0;
                      displayLcd(auth, 1);
                      outputPinHigh();
                      i2cPinHigh();
                  }
                  else {
                      ++attempts;
                      displayLcd(auth, 1);
                      outputPinLow();
                      i2cPinLow();
                  }
                  pin_code = "";
                  hidden_pin = "";
            }*/

        }

    }

    return 0;
}
