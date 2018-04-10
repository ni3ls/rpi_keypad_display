#include <sqlite3.h>
#include "config.h"
using namespace std;

string checkPassCode(string passcode) {
    Config config;
    loadConfig(config);

    sqlite3 *db;
    sqlite3_stmt *res;

    int rc = sqlite3_open(config.database.c_str(), &db);
    rc = sqlite3_prepare_v2(db, ("SELECT State FROM '" + config.tableDb + "' WHERE Pin = '" + passcode + "'").c_str(), -1, &res, 0);

    int step = sqlite3_step(res);

    string state;

    if(step == SQLITE_ROW) {
        state = "PIN OK";
     }
     else {
         state = "PIN ERROR";
    }

    // Update datetime('now')
    rc = sqlite3_prepare_v2(db, ("UPDATE '" + config.tableDb + "' SET Date = datetime('now') WHERE Pin = '" + passcode + "'").c_str(), -1, &res, 0);
    step = sqlite3_step(res);

    sqlite3_finalize(res);
    sqlite3_close(db);
    return state;
}
