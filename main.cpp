#include <iostream>
#include <string>
#include <unistd.h>
#include "GPIO.h"
#include "checkPassCode.h"
#include "keypad.h"

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

void keyTimeout() {
    Config config;
    loadConfig(config);

    ts = 0;
    while(true) {
        while(!stopTimer) {
            ts += 1;
            sleep(1);

           if(ts == key_timeout) {
               stopTimer = true;
               pin_code = "";
               hidden_pin = "";
               displayLcd("Sorry to slow",0);
               sleep(2);
               displayLcd("Welcome!", 0);
           }
        }
        ts = 0;
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

    while(true) {
        read(input, &ev, sizeof(ev));

        if(ev.type == 1 && ev.value == 1) {
            stopTimer = false;
            ts = 0;

            outputPinLow();
            i2cOutPinLow();

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

                    outputPinHigh();
                    i2cOutPinHigh();
                    stopTimer = true;
                    ts = 0;

                    // Box open
                    sleep(0.5);
                    while(i2cInput() != 0) {
                        // Do nothing
                    }

                    outputPinLow();
                    i2cOutPinLow();
                    pin_code = "";
                    hidden_pin = "";
                    displayLcd("Welcome", 0);
                }
                else {
                    displayLcd(auth, 1);
                    ++attempts;

                    stopTimer = true;
                    ts = 0;

                    outputPinLow();
                    i2cOutPinLow();
                }
                pin_code = "";
                hidden_pin = "";
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
