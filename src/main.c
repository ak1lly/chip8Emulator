#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    // Ensure a ROM is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path_to_rom>\n", argv[0]);
        return 1;
    }

    Chip8 chip8;
    chip8_init(&chip8);  // Initialize the CHIP-8 system

    load_rom(&chip8, argv[1]);  // Load the ROM
    int32_t speed=5;

    // Start the emulator loop
    while (1) {
        interface_handle_input(&chip8.interface);  // Handle input (keys)
        execute_cycle(&chip8);  // Execute the current opcode

        draw(&chip8);  // Draw to the screen if necessary

        if(speed<0)
		{
			speed = 0;
		}
		else
		{
			SDL_Delay(speed);
		}

        if (chip8.sound_timer > 0) {
            // Add sound functionality here if needed
        }
    }

    interface_destroy(&chip8.interface);  // Clean up SDL resources
    return 0;
}