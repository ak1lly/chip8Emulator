#include "PixelMap.h"



PixelMap :: PixelMap();
{
    memset(buffer, 0, sizeof(buffer));
}

bool updateBuffer(int x, int y, int val){
    buffer[x * y] = val;
}

uint8_t* getBuffer(){return buffer;}