#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include "interface.h"

#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16
#define STACK_SIZE 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define KEYPAD_SIZE 16
#define PROGRAM_START 0x200

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t V[NUM_REGISTERS];     // Registers V0 to VF
    uint16_t I;                   // Index register
    uint16_t pc;                  // Program counter
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;                   // Stack pointer
    uint32_t display[SCREEN_WIDTH * SCREEN_HEIGHT]; // 32-bit for color or monochrome
    uint8_t draw_flag;

    Interface interface;
} Chip8;

void chip8_init(Chip8 *chip8);
void load_rom(Chip8 *chip8, const char *filename);
void execute_cycle(Chip8 *chip8);
void draw(Chip8* chip8);
void reset(Chip8 *chip8);


#endif
