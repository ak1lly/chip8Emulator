#include <iostream>
#include <string.h>
#include "PixelMap.h"


const int PixelMap::operator[](int i){
    return buffer[i];
}

PixelMap::PixelMap(){
    memset(buffer, 0, sizeof(buffer));
    memset(pixels, 0xFF000000, sizeof(pixels));
}

bool PixelMap::updateBuffer(int index, int val){
    if ((index >= 0 && index < MAXWIDTH*MAXHEIGHT)){
        buffer[index] = val;
        return true;
    }
    return false;
}

bool PixelMap::updateBuffer(int x, int y, int val){
    if ((x >= 0 && x < MAXWIDTH) && (y >= 0 && y < MAXHEIGHT)){
        buffer[y * MAXWIDTH + x] = val;
        return true;
    }
    return false;
}

void PixelMap::resetBuffer(){
    memset(buffer, 0, sizeof(buffer));
}

void PixelMap::renderPixels(){
    resetPixels();
    for (int i = 0; i < MAXHEIGHT * MAXWIDTH; i++)
    {
        if(buffer[i] == 1){
            pixels[i] = 0xFFFFFFFF;
        } 
    }
}

const uint32_t* PixelMap::getPixels(){return pixels;}


void PixelMap::resetPixels(){
    memset(pixels, 0xFF000000, sizeof(pixels));
}

void PixelMap::clearScreen(){
    resetBuffer();
    resetPixels();
}

const int* PixelMap::getBuffer(){return buffer;}

void PixelMap::printBuffer(){
    for (int i = 0; i < MAXHEIGHT; ++i) {
        for (int j = 0; j < MAXWIDTH; ++j) {
            // std::cout << buffer[i*MAXWIDTH + j] << " ";
            if(buffer[i*MAXWIDTH + j] == 1){
                std::cout << buffer[i*MAXWIDTH + j] << " @"<<i<<","<<j<<std::endl;;
            }
        }
    }
}

