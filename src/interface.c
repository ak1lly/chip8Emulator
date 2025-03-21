#include "interface.h"
#include <stdio.h>
#include <string.h>

void interface_init(Interface *interface) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    interface->window = SDL_CreateWindow("CHIP-8 Emulator",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         SCREEN_WIDTH * SCALE,
                                         SCREEN_HEIGHT * SCALE,
                                         SDL_WINDOW_SHOWN);
    if (!interface->window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    interface->renderer = SDL_CreateRenderer(interface->window, -1, SDL_RENDERER_ACCELERATED);
    if (!interface->renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(interface->window);
        SDL_Quit();
        exit(1);
    }

    // Clear keypad state
    memset(interface->keypad, 0, sizeof(interface->keypad));
}

void interface_draw(Interface *interface, uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT]) {
    SDL_SetRenderDrawColor(interface->renderer, 0, 0, 0, 255);
    SDL_RenderClear(interface->renderer);

    SDL_SetRenderDrawColor(interface->renderer, 255, 255, 255, 255);

    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (screen[y * SCREEN_WIDTH + x]) {
                SDL_Rect rect = {
                    x * SCALE,
                    y * SCALE,
                    SCALE,
                    SCALE
                };
                SDL_RenderFillRect(interface->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(interface->renderer);
}

void interface_handle_input(Interface *interface) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            int value = (event.type == SDL_KEYDOWN) ? 1 : 0;

            switch (event.key.keysym.sym) {
                case SDLK_1: interface->keypad[0x1] = value; break;
                case SDLK_2: interface->keypad[0x2] = value; break;
                case SDLK_3: interface->keypad[0x3] = value; break;
                case SDLK_4: interface->keypad[0xC] = value; break;
                case SDLK_q: interface->keypad[0x4] = value; break;
                case SDLK_w: interface->keypad[0x5] = value; break;
                case SDLK_e: interface->keypad[0x6] = value; break;
                case SDLK_r: interface->keypad[0xD] = value; break;
                case SDLK_a: interface->keypad[0x7] = value; break;
                case SDLK_s: interface->keypad[0x8] = value; break;
                case SDLK_d: interface->keypad[0x9] = value; break;
                case SDLK_f: interface->keypad[0xE] = value; break;
                case SDLK_z: interface->keypad[0xA] = value; break;
                case SDLK_x: interface->keypad[0x0] = value; break;
                case SDLK_c: interface->keypad[0xB] = value; break;
                case SDLK_v: interface->keypad[0xF] = value; break;
            }
        }
    }
}

void interface_destroy(Interface *interface) {
    SDL_DestroyRenderer(interface->renderer);
    SDL_DestroyWindow(interface->window);
    SDL_Quit();
}
