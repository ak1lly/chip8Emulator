#ifndef KEYPAD_H
#define KEYPAD_H

#include <cstdint> 
#include <SDL2/SDL.h>
#define NUM_KEYS 16

class Keypad 
{
    public:
        const int operator[](int);

        Keypad();
        bool input();
        void handle_key_down(uint8_t);
        void handle_key_up(uint8_t);

        //helper for logging
        void print_array(const int*, int);
    protected:
    private:
        int keypad[NUM_KEYS];
};

#endif