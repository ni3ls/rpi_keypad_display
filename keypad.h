#ifndef KEYPAD_H
#define KEYPAD_H

// /dev/input : linux/input.h
#define KEYPAD "/dev/input/event0"

int keypadValue(int key_code);

#endif
