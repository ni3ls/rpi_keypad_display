#ifndef VERIFY_PCODE_H
#define VERIFY_PCODE_H

#include <string>

class VerifyPCode {
    public:
        VerifyPCode();
        std::string verifyPinCode(const std::string &pincode);

    private:
        std::string pinStat;
};

#endif
