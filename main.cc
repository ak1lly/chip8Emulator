#include <SDL2/SDL.h>
#include <thread>
#include <chrono>
#include <cstdint>
#include <iostream>
#include "Keypad.h"
#include "PixelMap.h"


using namespace std;

void runSimulationCycle(PixelMap*, int, int);
void updateRenderer(SDL_Texture* , SDL_Renderer* , const uint32_t*);

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout<< "SDL_Init Error: %s\n"<< SDL_GetError()<< endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Window* window = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    if (window == NULL) {
        cout<< "SDL_CreateWindow Error: %s\n"<< SDL_GetError()<< endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        cout<<"SDL_CreateRenderer Error: %s\n"<< SDL_GetError() <<endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    if (texture == nullptr)
    {
        cout<<"SDL_CreateTexture Error: %s\n"<< SDL_GetError() <<endl;
        SDL_Quit();
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    PixelMap screen;

    //this is how we interact with the screen (using PixelMap)

    for(int i = 0; i<MAXHEIGHT; i++){
        for(int j = 0; j<MAXWIDTH; j++){
            runSimulationCycle(&screen, j, i);
            screen.renderPixels();
            screen.printBuffer();
            updateRenderer(texture, renderer, screen.getPixels());
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    Keypad keypad = Keypad();

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(1);
            }
            if (event.type == SDL_KEYDOWN){
                keypad.handle_key_down(event.key.keysym.sym);
            }
            if (event.type == SDL_KEYUP){
                keypad.handle_key_up(event.key.keysym.sym);
            }
        }
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void updateRenderer(SDL_Texture* tex, SDL_Renderer* ren, const uint32_t* map){
    int width, height;
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);

    int pitch = width * sizeof(uint32_t);

    SDL_UpdateTexture(tex, NULL, map, pitch);

    //update render with new texture

    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
}