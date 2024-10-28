#ifndef PIXELMAP_H
#define PIXELMAP_H

#include <cstdint> 

#define MAXHEIGHT 32
#define MAXWIDTH 64


/*
* @brief A class representing an the mapping that will emulate the current output to the 64 x 32 screen of a chip8 interface.
*/
class PixelMap 
{
    public:
        PixelMap();
        bool updateBuffer();
        uint8_t* getBuffer();
    private:
        uint8_t buffer[MAXHEIGHT * MAXWIDTH];
};

#endif