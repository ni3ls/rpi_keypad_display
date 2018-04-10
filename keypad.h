#ifndef KEYPAD_H
#define KEYPAD_H

#include <vector>
using namespace std;

// /dev/input : linux/input.h
#define KEYPAD "/dev/input/event0"

int keypadValue(int key_code);
vector<char> keypadDel(vector<char> &a);

#endif
