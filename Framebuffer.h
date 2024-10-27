#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint> 
#include <unordered_map>

#define MAXHEIGHT 32
#define MAXWIDTH 64


/*
* @brief A class representing an object that will emulate the current output to the 64 x 32 screen of a chip8 interface.
*/
class Framebuffer 
{
    public:
        Framebuffer();
        bool updateBuffer();
    private:
        uint8_t buffer[MAXHEIGHT, MAXWIDTH];
};

#endif;