#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    std::string database;
    std::string tableDb;
    bool hide_char;
    std::string hidden_char;
    int retention;
    int attempts;
    int max_digits;
    int key_pause;
    int drawer_open_time;
    int drawer_open_err_time;
};

void loadConfig(Config& config);

#endif
