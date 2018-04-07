#ifndef CONFIG_H
#define CONFIG_H

#include <string>
using namespace std;

struct Config {
    string database;
    string tableDb;
    bool hide_char;
    char hidden_char;
    int retention;
    int attempts;
    int max_digits;
    int key_pause;
};

void loadConfig(Config& config);

#endif
