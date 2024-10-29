#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint> 
#include <string>
#include <unordered_map>
#include "PixelMaps.h"

class chip8 
{
    public:
        chip8();
        bool cycle();
    protected:
    private:
        //CPU
        unordered_map<string, uint8_t> registers;
        uint16_t index_reg, program_counter;
        uint16_t stack[32];
        uint8_t stack_ptr;

        //Timers
        uint8_t delay_t;
        uint8_t sound_t;
    
        // Memory, 4KB
        uint8_t ram[4096]

        //Graphics emulation
        PixelMap screen;

        //Keypad
        Keypad Keypad
};

#endif;