#include <iostream>
#include "keypad.h"
#include <unistd.h>
#include "config.h"
#include <string>
using namespace std;

int main() {
    Config conf;
    loadConfig(conf);

    Keypad keyP(conf.retention);
    keyP.startKeyTh();
    keyP.sethideCode(conf.hide_char);
    keyP.sethiddenChar(conf.hidden_char);

    while(true) {
        keyP.readKeypad();
    }
}
