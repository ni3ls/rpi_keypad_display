#include <sqlite3.h>
#include "verifyPCode.h"
#include "config.h"
#include <iostream>
#include "io.h"

using namespace std;

IO pi_IO;

VerifyPCode::VerifyPCode() {
}

string VerifyPCode::verifyPCode(const string &pinCode) {
    Config config;
    loadConfig(config);
    sqlite3 *db;
    sqlite3_stmt *res;

    int rc = sqlite3_open(config.database.c_str(), &db);
    rc = sqlite3_prepare_v2(db, ("SELECT State FROM '" + config.tableDb + "' WHERE Pin = '" + pinCode + "'").c_str(), -1, &res, 0);

    int step = sqlite3_step(res);

    if(step == SQLITE_ROW) {
        pStat = "PIN OK";
        pi_IO.displayLcd("PIN OK", 1);

        // Update datetime('now')
        rc = sqlite3_prepare_v2(db, ("UPDATE '" + config.tableDb + "' SET Date = datetime('now') WHERE Pin = '" + pinCode + "'").c_str(), -1, &res, 0);
        step = sqlite3_step(res);
    }
    else {
        pStat = "PIN ERROR";
        pi_IO.displayLcd("PIN ERROR", 1);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
    return pStat;
}

