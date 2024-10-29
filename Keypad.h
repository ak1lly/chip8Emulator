#ifndef KEYPAD_H
#define KEYPAD_H

#include <cstdint> 
#include <SDL.h>
#define NUM_KEYS 16

class Keypad 
{
    public:
        Keypad();
        bool input();
        void handle_key_down(uint8_t);
        void handle_key_up(uint8_t);
    protected:
    private:
        int keypad[NUM_KEYS];
};

#endif