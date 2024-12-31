#include "chip8.h"
#include <SDL2/SDL.h>

using namespace std;


unsigned char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8::chip8(){
    memset(V, 0, sizeof(V));
    uint16_t index = 0;           // Index register, typically starts at 0
    uint16_t program_counter = 0x200; // 0x000 to 0x1FF is reserved for  interpreter // Program counter typically starts at 0x200
    memset(stack, 0, sizeof(stack));    // Initialize stack elements to 0
    uint8_t stack_ptr = 0;       // Stack pointer starts at 0

    // Timers
    uint8_t delay_t = 0;         // Delay timer starts at 0
    uint8_t sound_t = 0;         // Sound timer starts at 0

    // ram (RAM)
    memset(ram, 0, sizeof(ram)); // Initialize all memory to 0
    for (int i = 0; i < 80; i++)
    {
        ram[i] = chip8_fontset[i];
    }

    screen = PixelMap();
    keypad = Keypad();

}

bool chip8::loadROM(char* rom_name){
    FILE *rom = fopen(rom_name, "rb");
    if(!rom){
        SDL_Log("ROM file \"%s\" is invalid or does not exist", rom_name);
        return false;
    }

    //seeks out the end of the rom
    fseek(rom, 0, SEEK_END);
    //return the current postion of on the rom (which is at the end now)
    const size_t rom_size = ftell(rom); 
    const size_t max_size = sizeof ram - 0x200;

    if(max_size<rom_size){
        SDL_Log("ROM file \"%s\" size %zu is too large for max size %zu", rom_name, rom_size, max_size);
        return false;
    }

    //move postion in ROM back to beginning
    rewind(rom);

    if (fread(&ram[0x200], rom_size, 1, rom) != 1){
        SDL_Log("Failed to read ROM file \"%s\"", rom_name):
        return false;
    }

    //close file stream
    fclose(rom);



    return true;
}

void chip8::cycle(){
    int opcode = (ram[program_counter] << 8) | (ram[program_counter + 1]);
    int opcode_msb_nibble = get_nibble(opcode, 12, 0xF000); //if value is ABCD(each 4 bits), it returns A
    int val, reg, reg1, reg2;

    if (trace_mode)
    {
        printf("%04X %04X %02X ", program_counter, opcode, stack_ptr);
        for (int i = 0; i < 15; i++)
        {
            printf("%.2X ", V[i]);
        }
        printf("\n");
    }

    switch (opcode_msb_nibble)
    {
        case 0:
            //only found 00E0 and 00EE starting with 0 so check for these two
            switch (opcode)
            {
                case 0x00E0:
                    screen.clearScreen();
                    draw_flag = true;
                    program_counter += 2;
                    break;

                case 0x00EE:
                    stack_ptr --;
                    program_counter = stack[stack_ptr ];
                    program_counter += 2;
                    break;

                default:
                    //incorrect opcode
                    SDL_Log("Invalid opcode: %04X", opcode);
                    break;
            }
            break;

        case 1:
            program_counter = opcode & 0x0FFF;
            break;

        case 2:
            //only instruction is 2NNN Calls subroutine at NNN.
            //so put current address in stack and move pc to NNN
            stack[stack_ptr] = program_counter;
            stack_ptr++;
            program_counter = opcode & 0x0FFF;
            break;

        case 3:
            //only instruction is 3XNN Skips the next instruction if VX equals NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            program_counter += 2; //next instruction
            if (V[reg] == val)
            {
                program_counter += 2; //adding 2 again so this instruction is skipped
            }
            break;

        case 4:
            //instruction is 4XNN. Skips the next instruction if VX doesn't equal NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            program_counter += 2; //next instruction
            if (V[reg] != val)
            {
                program_counter += 2; //adding 2 again so this instruction is skipped
            }
            break;

        case 5:
            //instruction is 5XY0. Skips the next instruction if VX equals VY.
            reg1 = get_nibble(opcode, 8, 0x0F00);
            reg2 = get_nibble(opcode, 4, 0x00F0);
            program_counter += 2;
            if (V[reg1] == V[reg2])
            {
                program_counter += 2;
            }
            break;

        case 6:
            //instruction is 6XNN. Sets VX to NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            V[reg] = val;
            program_counter += 2;
            break;

        case 7:
            //instruction is 7XNN. Adds NN to VX.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            V[reg] += val;
            program_counter += 2;
            break;

        case 8:
            //multiple instructions possible
            val = get_nibble(opcode, 0, 0x000F); //extract last 4 bits
            switch (val)
            {
                case 0:
                    //8XY0. Sets VX to the value of VY.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] = V[reg2];
                    program_counter += 2;
                    break;

                case 1:
                    //8XY1. Sets VX to VX or VY. (Bitwise OR operation) VF is reset to 0.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] |= V[reg2];
                    V[0xF] = 0;
                    program_counter += 2;
                    break;

                case 2:
                    //8XY2. Sets VX to VX and VY. (Bitwise AND operation) VF is reset to 0.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] &= V[reg2];
                    V[0xF] = 0;
                    program_counter += 2;
                    break;

                case 3:
                    //8XY3. Sets VX to VX xor VY. VF to 0
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] ^= V[reg2];
                    V[0xF] = 0;
                    program_counter += 2;
                    break;

                case 4:
                    //Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    if (V[reg1] + V[reg2] > 0xFF)
                    {
                        V[0xF] = 1;
                    }
                    else
                    {
                        V[0xF] = 0;
                    }
                    V[reg1] += V[reg2];
                    V[reg1] = (int8_t) V[reg1];
                    program_counter += 2;
                    break;

                case 5:
                    //VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    if (V[reg1] < V[reg2])
                    {
                        V[0xF] = 0;
                    }
                    else
                    {
                        V[0xF] = 1;
                    }
                    V[reg1] = (uint8_t) V[reg1] - (uint8_t) V[reg2];
                    program_counter += 2;
                    break;

                case 6:
                    //Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    V[0xF] = V[reg] & 0x1;
                    V[reg] >>= 1;
                    V[reg] = (uint8_t) V[reg];
                    program_counter += 2;
                    break;

                case 7:
                    //Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    if (V[reg1] > V[reg2])
                    {
                        V[0xF] = 0;
                    }
                    else
                    {
                        V[0xF] = 1;
                    }
                    V[reg1] = (uint8_t) V[reg2] - (uint8_t) V[reg1];
                    program_counter += 2;
                    break;

                case 0xE:
                    //Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.[2]
                    reg = get_nibble(opcode, 8, 0x0F00);
                    V[0xF] = V[reg] >> 7;
                    V[0xF] = (uint8_t) V[0xF];
                    V[reg] <<= 1;
                    V[reg] = (uint8_t) V[reg];
                    program_counter += 2;
                    break;

                default:
                    SDL_Log("Invalid opcode: %04X", opcode);
                    break;
            }
            break;

        case 9:
            //9XY0. 	Skips the next instruction if VX doesn't equal VY.
            reg1 = get_nibble(opcode, 8, 0x0F00);
            reg2 = get_nibble(opcode, 4, 0x00F0);
            program_counter += 2;
            if (V[reg1] != V[reg2])
            {
                program_counter += 2;
            }
            break;

        case 10: //0xA
            //ANNN Sets I to the address NNN.
            index = opcode & 0x0FFF;
            program_counter += 2;
            break;

        case 11: //0xB
            //BNNN. Jumps to the address NNN plus V0.
            program_counter = (opcode & 0x0FFF);
            program_counter += V[0];
            break;

        case 12: //0xC
            //CXNN. Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            V[reg] = 5 & val;
            program_counter += 2;
            break;

        case 13: //0xD
        {
            //DXYN. Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
            // Each row of 8 pixels is read as bit-coded starting from memory location I;
            // I value doesn’t change after the execution of this instruction.
            // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen

            reg1 = get_nibble(opcode, 8, 0x0F00);
            reg2 = get_nibble(opcode, 4, 0x00F0);
            int x = V[reg1], y = V[reg2];
            int ht = opcode & 0x000F; //N
            int wt = 8;
            V[0x0F] = 0;

            for (int i = 0; i < ht; i++)
            {
                int pixel = ram[index + i];
                for (int j = 0; j < wt; j++)
                {
                    if ((pixel & (0x80 >> j)) != 0)
                    {
                        int index = ((x + j) + ((y + i) * 64)) % 2048;
                        if (screen[index] == 1)
                        {
                            V[0x0F] = 1;
                        }
                        screen.updateBuffer(index, screen[index^1]);
                    }
                }
            }

            draw_flag = true;
            program_counter += 2;
            break;
        }
        case 14: //0x0E
            //two instructions possible
            val = get_nibble(opcode, 0, 0x00FF);
            switch (val)
            {
                case 0x9E:
                    //Skips the next instruction if the key stored in VX is pressed.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    program_counter += 2;
                    if (keypad[V[reg]] != 0)
                    {
                        program_counter += 2;
                    }
                    break;

                case 0xA1:
                    //Skips the next instruction if the key stored in VX isn't pressed.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    program_counter += 2;
                    if (keypad[V[reg]] == 0)
                    {
                        program_counter += 2;
                    }
                    break;

                default:
                    SDL_Log("Invalid opcode: %04X", opcode);
                    break;

            }
            break;

        case 15: //0x0F
            //multiple instructions possible
            val = get_nibble(opcode, 0, 0x00FF);
            switch (val)
            {
                case 0x07:
                    //FX07. Sets VX to the value of the delay timer.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    V[reg] = delay_t;
                    program_counter += 2;
                    break;

                case 0x0A:
                    //FX0A. A key press is awaited, and then stored in VX.
                {
                    bool key_pressed = false;
                    reg = get_nibble(opcode, 8, 0x0F00);
                    for (int i = 0; i < 16; i++)
                    {
                        if (keypad[i] != 0)
                        {
                            key_pressed = true;
                            V[reg] = (uint8_t) i;
                        }
                    }
                    if (key_pressed)
                    {
                        program_counter += 2;
                    }
                    break;
                }

                case 0x15:
                    //FX15. Sets the delay timer to VX.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    delay_t = V[reg];
                    program_counter += 2;
                    break;

                case 0x18:
                    //sets the sound timer to VX
                    reg = get_nibble(opcode, 8, 0x0F00);
                    sound_t = V[reg];
                    program_counter += 2;
                    break;

                case 0x1E:
                    //Adds VX to I
                    reg = get_nibble(opcode, 8, 0x0F00);
                    if (index + V[reg] > 0xFFF)
                    {
                        V[0xF] = 1;
                    }
                    else
                    {
                        V[0xF] = 0;
                    }
                    index += V[reg];
                    index = (uint16_t) index;
                    program_counter += 2;
                    break;

                case 0x29:
                    //Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    index = V[reg] * 0x5;
                    program_counter += 2;
                    break;

                case 0x33:
                    //Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
                    reg = get_nibble(opcode, 8, 0x0F00);
                    ram[index] = (uint8_t) ((uint8_t) V[reg] / 100);
                    ram[index + 1] = (uint8_t) ((uint8_t) (V[reg] / 10) % 10);
                    ram[index + 2] = (uint8_t) ((uint8_t) (V[reg] % 100) % 10);
                    program_counter += 2;
                    break;

                case 0x55:
                    //Stores V0 to VX (including VX) in memory starting at address I
                    reg = get_nibble(opcode, 8, 0x0F00);
                    for (int i = 0; i <= reg; i++)
                    {
                        ram[index + i] = V[i];
                    }
                    index = index + reg + 1;
                    index = (uint16_t) index;
                    program_counter += 2;
                    break;

                case 0x65:
                    //Fills V0 to VX (including VX) with values from memory starting at address I
                    reg = get_nibble(opcode, 8, 0x0F00);
                    for (int i = 0; i <= reg; i++)
                    {
                        V[i] = ram[index + i];
                    }
                    index = index + reg + 1;
                    index = (uint16_t) index;
                    program_counter += 2;
                    break;

                default:
                    SDL_Log("Invalid opcode: %04X", opcode);
                    break;
            }
            break;

        default:
            SDL_Log("Invalid opcode: %04X", opcode);
            break;
    }

    if (delay_t > 0)
    {
        delay_t--;
    }
    if (sound_t > 0)
    {
        if (sound_on)
        {
            printf("\a\n"); //terminal dependent, should be changed later
        }
        sound_t--;
    }
}

int chip8::get_nibble(int val, int bits, int val_to_binary_and = 0xFFFF) //extracts 4 bits from val
{
    return ((val & val_to_binary_and) >> bits);
}