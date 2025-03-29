#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chip8_init(Chip8 *chip8) {
    chip8->pc = PROGRAM_START;
    chip8->I = 0;
    chip8->sp = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    memset(chip8->memory, 0, sizeof(chip8->memory));
    memset(chip8->V, 0, sizeof(chip8->V));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->display, 0, sizeof(chip8->display));
    // chip8->display[120] = 1;
    chip8->draw_flag = 0;

    // Initialize display and input
    interface_init(&chip8->interface);

    // Load font set into memory (0x000 to 0x050)
    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0  // 9
    };
    memcpy(chip8->memory, fontset, sizeof(fontset));
}

void load_rom(Chip8 *chip8, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open ROM: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fread(&chip8->memory[0x200], 1, MEMORY_SIZE - 0x200, file);
    fclose(file);
}

//works
void draw(Chip8* chip8)
{		
	if (chip8->draw_flag == 1)
	{
		interface_draw(&chip8->interface, chip8->display);
	}
	chip8->draw_flag = 0;
}

void execute_cycle(Chip8 *chip8){
    // Fetch opcode (combine two 8-bit memory locations into a single 16-bit opcode)
    uint16_t opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;

    uint8_t X, Y, N, NN;
	uint16_t NNN;

    X = (opcode & 0x0F00) >> 8;
	Y = (opcode & 0x00F0) >> 4;
	NNN = (opcode & 0x0FFF);
	NN = (opcode & 0x00FF);
	N = (opcode & 0x000F);


    // Decode and execute
    switch (opcode & 0xF000) {
        // 0x0NNN - Calls RCA 1802 program at address NNN (ignored by most emulators)
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: 
                    // 0x00E0 - Clear the display
                    memset(chip8->display, 0, sizeof(chip8->display));
                    chip8->draw_flag = 1;
                    break;
                
                case 0x00EE:
                    // 0x00EE - Return from subroutine
                    // TODO: Pop stack and set PC
                    chip8->sp--;
				    chip8->pc = chip8->stack[chip8->sp];
                    break;

                default:
                    // 0x0NNN - Execute machine language subroutine at address NNN
                    // TODO: Ignore for most emulators
                    break;
            }
            break;

        // 0x1NNN - Jump to address NNN
        case 0x1000:
            // TODO: Set program counter to NNN
            chip8->pc = (opcode & 0x0FFF);
            break;

        // 0x2NNN - Call subroutine at NNN
        case 0x2000:
            // TODO: Push PC to stack and jump to NNN
            chip8->stack[chip8->sp] = chip8->pc;
            chip8->sp++;
            chip8->pc = (opcode & 0x0FFF);
            break;

        // 0x3XNN - Skip next instruction if VX == NN
        case 0x3000:
            // TODO: Skip instruction if VX == NN
            if(chip8->V[X] == NN) chip8->pc+=2;
            break;

        // 0x4XNN - Skip next instruction if VX != NN
        case 0x4000:
            // TODO: Skip instruction if VX != NN
            if(chip8->V[X] != NN) chip8->pc+=2;
            break;

        // 0x5XY0 - Skip next instruction if VX == VY
        case 0x5000:
            // TODO: Skip instruction if VX == VY
            if(chip8->V[X] == chip8->V[Y]) chip8->pc+=2;
            break;

        // 0x6XNN - Set VX = NN
        case 0x6000:
            // TODO: Set VX to NN
            chip8->V[X] = NN;
            break;

        // 0x7XNN - Set VX = VX + NN
        case 0x7000:
            // TODO: Add NN to VX (without carry flag)
            chip8->V[X] += NN;
            break;

        // 0x8XY_ - ALU operations
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    // 0x8XY0 - Set VX = VY
                    // TODO: Set VX to VY
                    chip8->V[X] = chip8->V[Y]; 
                    break;
                
                case 0x0001:
                    // 0x8XY1 - Set VX = VX OR VY
                    // TODO: Bitwise OR
                    chip8->V[X] |= chip8->V[Y]; 
                    break;

                case 0x0002:
                    // 0x8XY2 - Set VX = VX AND VY
                    // TODO: Bitwise AND
                    chip8->V[X] &= chip8->V[Y]; 
                    break;

                case 0x0003:
                    // 0x8XY3 - Set VX = VX XOR VY
                    // TODO: Bitwise XOR
                    chip8->V[X] ^= chip8->V[Y];
                    break;

                case 0x0004:
                    // 0x8XY4 - Add VY to VX (set VF for carry)
                    // TODO: Add with carry
                    int carry = (int)(chip8->V[X]) + (int)(chip8->V[Y]);
                    chip8->V[0xF] = (carry>255) ? 1 : 0;
                    chip8->V[X] = carry & 0xFF;
                    break;

                case 0x0005:
                    // 0x8XY5 - Subtract VY from VX (set VF for NOT borrow)
                    // TODO: Subtract with borrow
                    chip8->V[0xF] = (chip8->V[X]>chip8->V[Y]) ? 1 : 0;
                    chip8->V[X] -= chip8->V[Y];
                    break;

                case 0x0006:
                    // 0x8XY6 - Right shift VX (set VF for least significant bit)
                    // TODO: Right shift VX
                    chip8->V[0xF] = chip8->V[X] & 1;
				    chip8->V[X] >>= 1;
                    break;

                case 0x0007:
                    // 0x8XY7 - Set VX = VY - VX (set VF for NOT borrow)
                    // TODO: Subtract reverse
                    chip8->V[0xF] = (chip8->V[Y]>chip8->V[X]) ? 1 : 0;
                    chip8->V[X] = chip8->V[Y] - chip8->V[X];
                    break;

                case 0x000E:
                    // 0x8XYE - Left shift VX (set VF for most significant bit)
                    // TODO: Left shift VX
                    chip8->V[0xF] = chip8->V[X] >> 7;
				    chip8->V[X] <<= 1;
                    break;
            }
            break;

        // 0x9XY0 - Skip next instruction if VX != VY
        case 0x9000:
            // TODO: Skip instruction if VX != VY
            if(chip8->V[X]!=chip8->V[Y]) chip8->pc+=2;
            break;

        // 0xANNN - Set I = NNN
        case 0xA000:
            // TODO: Set I to NNN
            chip8->I = NNN;
            break;

        // 0xBNNN - Jump to address NNN + V0
        case 0xB000:
            // TODO: Jump to NNN + V0
            chip8->pc = (NNN) + chip8->V[0x0];
            break;

        // 0xCXNN - Set VX = random byte AND NN
        case 0xC000:
            // TODO: Generate random byte and AND with NN
            chip8->V[X] = (rand() % 0x100) & (NN);
            break;

        // 0xDXYN - Draw sprite at coordinate (VX, VY)
        case 0xD000:
            // TODO: Draw sprite and set VF for collision
            uint16_t x = chip8->V[X];
            uint16_t y = chip8->V[Y];
            uint16_t height = N;
            uint8_t pixel;

            chip8->V[0xF] = 0;
            for (int yaxis = 0; yaxis < height; yaxis++){
                pixel = chip8->memory[chip8->I+yaxis];
                for (int xaxis = 0; xaxis < 8; xaxis++){
                    if((pixel & (0x80 >> xaxis)) != 0) {
						if(chip8->display[(x + xaxis + ((y + yaxis) * 64))] == 1){
							chip8->V[0xF] = 1;                                   
						}
						chip8->display[x + xaxis + ((y + yaxis) * 64)] ^= 1;
					}
                }
            }
            chip8->draw_flag = 1;

            break;

        // 0xEX__ - Key handling
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x9E:
                    // 0xEX9E - Skip next instruction if key in VX is pressed
                    // TODO: Check if key is pressed
                    if(chip8->interface.keypad[chip8->V[X]] != 0){
                        chip8->pc+=2;}
                    break;

                case 0xA1:
                    // 0xEXA1 - Skip next instruction if key in VX is NOT pressed
                    // TODO: Check if key is NOT pressed
                    if(chip8->interface.keypad[chip8->V[X]] == 0){
                        chip8->pc+=2;}
                    break;
            }
            break;

        // 0xFX__ - Miscellaneous instructions
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x07:
                    // 0xFX07 - Set VX = delay timer value
                    // TODO: Get delay timer
                    chip8->V[X] = chip8->delay_timer;
                    break;

                case 0x0A:
                    // 0xFX0A - Wait for key press and store it in VX
                    // TODO: Wait for key press
                    int press_check = 0;
                    for(int i = 0; i<KEYPAD_SIZE; i++){
                        if(chip8->interface.keypad[i]){
                            press_check = 1;
                            chip8->V[X] = i;
                        }
                    }

                    if(press_check == 0) chip8->pc -= 2;
                    break;

                case 0x15:
                    // 0xFX15 - Set delay timer = VX
                    // TODO: Set delay timer
                    chip8->delay_timer = chip8->V[X];
                    break;

                case 0x18:
                    // 0xFX18 - Set sound timer = VX
                    // TODO: Set sound timer
                    chip8->sound_timer = chip8->V[X];
                    break;

                case 0x1E:
                    // 0xFX1E - Set I = I + VX
                    // TODO: Increment I by VX
                    chip8->I = chip8->I + chip8->V[X];
                    break;

                case 0x29:
                    // 0xFX29 - Set I = location of sprite for digit VX
                    // TODO: Get sprite location
                    chip8->I = chip8->V[X] * 5;
                    break;

                case 0x33:
                    // 0xFX33 - Store BCD of VX in memory
                    // TODO: Store BCD
                    int vX;
					vX = chip8->V[X];
					chip8->memory[chip8->I] = (vX - (vX % 100)) / 100;
					vX -= chip8->memory[chip8->I] * 100;
					chip8->memory[chip8->I + 1] = (vX - (vX % 10)) / 10;
					vX -= chip8->memory[chip8->I + 1] * 10;
					chip8->memory[chip8->I + 2] = vX;
                    break;

                case 0x55:
                    // 0xFX55 - Store registers V0 to VX in memory starting at I
                    // TODO: Store registers
                    for (uint8_t i = 0; i <= X; ++i){
                        chip8->memory[chip8->I + i] = chip8->V[i];	
                    }
                    break;

                case 0x65:
                    // 0xFX65 - Read registers V0 to VX from memory starting at I
                    // TODO: Read registers
                    for (uint8_t i = 0; i <= X; ++i){
                        chip8->V[i] = chip8->memory[chip8->I + i];	
                    }
                    break;
            }
            break;

        default:
            printf("Unknown opcode: 0x%04X\n", opcode);
            break;
    }

    //Timers update
    if (chip8->delay_timer > 0) chip8->delay_timer--;
    if (chip8->sound_timer > 0) chip8->sound_timer--;
}

void reset(Chip8 *chip8) {
    chip8_init(chip8);
}