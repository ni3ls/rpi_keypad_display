#include <fstream>
#include <sstream>
#include "config.h"
using namespace std;

void loadConfig(Config& config) {
    ifstream file(".env");
    string line;

    while(getline(file, line)) {
        istringstream data(line.substr(line.find("=") + 1));

        if(line.find("DATABASE") != -1)
            data >> config.database;

        if(line.find("TABLE_DB") != -1)
            data >> config.tableDb;

        if(line.find("HIDE_CHAR") != -1)
            data >> config.hide_char;

        if(line.find("HIDDEN_CHAR") != -1)
            data >> config.hidden_char;

        if(line.find("RETENTION") != -1)
            data >> config.retention;

        if(line.find("ATTEMPTS") != -1)
            data >> config.attempts;

        if(line.find("MAX_DIGITS") != -1)
            data >> config.max_digits;

        if(line.find("KEY_PAUSE") != -1)
            data >> config.key_pause;
    }
}

