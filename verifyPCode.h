#ifndef VERIFY_PCODE_H
#define VERIFY_PCODE_H

#include <string>
using namespace std;

class VerifyPCode {
    public:
        VerifyPCode();
        ~VerifyPCode() {}
        string verifyPCode(const string &pincode);

    private:
        string pStat;
};

#endif
