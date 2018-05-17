#ifndef DRAWER_H
#define DRAWER_H

#include <thread>

class Drawer {
    public:
        Drawer();
        void startDrawerTh();
        void openDrawer(int drawerNum);

    private:
        int _interval;
        int _drawerNum;
        int _event;
        std::thread _tdrawer;
        void timerDrawerTh();
        int drawerStatus;
        int _pinA0;
        int _pinA1;
};

#endif
