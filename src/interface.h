#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <stdint.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCALE 10 // Scale factor for enlarging the display

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint8_t keypad[16];  // 16 keys for CHIP-8 keypad state
} Interface;

// Initialize the display and keypad
void interface_init(Interface *interface);

// Draw the CHIP-8 screen buffer
void interface_draw(Interface *interface, uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT]);

// Handle input and update the CHIP-8 keypad state
void interface_handle_input(Interface *interface);

// Cleanup and close the display
void interface_destroy(Interface *interface);

#endif // INTERFACE_H
