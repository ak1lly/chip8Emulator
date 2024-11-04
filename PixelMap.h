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
        const int operator[](int);

        PixelMap();

        //buffer arr work
        bool updateBuffer(int, int, int);
        void resetBuffer();

        //pixel work
        void renderPixels();
        const uint32_t* getPixels();
        void resetPixels();

        void clearScreen();

        const int* getBuffer();

        void printBuffer();

        void runSimulation();
    private:
        int buffer[MAXHEIGHT * MAXWIDTH];
        uint32_t pixels[MAXHEIGHT * MAXWIDTH];

};

#endif