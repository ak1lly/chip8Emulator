#include <SDL.h>
#include <cstdint>
#include <iostream>

using namespace std;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout<< "SDL_Init Error: %s\n"<< SDL_GetError()<< endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Window* window = SDL_CreateWindow("tester", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
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

    uint8_t screen[32*64];
    memset(screen, 0, sizeof(screen));

    screen[16 * 32] = 1;

    //this is how we interact with the screen (using PixelMap)
    uint32_t pixels[32 * 64];
    for (int i = 0; i < 32 * 64; i++)
    {
        if (screen[i] == 0)
        {
            pixels[i] = 0xFF000000;
        }
        else
        {
            pixels[i] = 0xFFFFFFFF;
        }
    }

    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));

    //update render with new texture

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(20000); // Wait for 2 seconds

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}