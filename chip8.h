#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint> 
#include <string>
#include <unordered_map>
#include "PixelMap.h"
#include "Keypad.h"


class chip8 
{
    public:
        chip8();
        bool loadProgram(std::string);
        bool cycle();

        //opcode handlers
        void handle00E0(); // CLS - Clear display
        void handle00EE(); // RET - Return from subroutine
        void handle1NNN(uint16_t addr); // JP addr - Jump to address NNN
        void handle2NNN(uint16_t addr); // CALL addr - Call subroutine at NNN
        void handle3XNN(uint8_t Vx, uint8_t byte); // SE Vx, byte - Skip next instruction if Vx == NN
        void handle4XNN(uint8_t Vx, uint8_t byte); // SNE Vx, byte - Skip next instruction if Vx != NN
        void handle5XY0(uint8_t Vx, uint8_t Vy); // SE Vx, Vy - Skip next instruction if Vx == Vy
        void handle6XNN(uint8_t Vx, uint8_t byte); // LD Vx, byte - Set Vx = NN
        void handle7XNN(uint8_t Vx, uint8_t byte); // ADD Vx, byte - Add NN to Vx
        void handle8XY0(uint8_t Vx, uint8_t Vy); // LD Vx, Vy - Set Vx = Vy
        void handle8XY1(uint8_t Vx, uint8_t Vy); // OR Vx, Vy - Set Vx = Vx OR Vy
        void handle8XY2(uint8_t Vx, uint8_t Vy); // AND Vx, Vy - Set Vx = Vx AND Vy
        void handle8XY3(uint8_t Vx, uint8_t Vy); // XOR Vx, Vy - Set Vx = Vx XOR Vy
        void handle8XY4(uint8_t Vx, uint8_t Vy); // ADD Vx, Vy - Add Vy to Vx, set VF = carry
        void handle8XY5(uint8_t Vx, uint8_t Vy); // SUB Vx, Vy - Subtract Vy from Vx, set VF = NOT borrow
        void handle8XY6(uint8_t Vx); // SHR Vx - Shift Vx right by 1, set VF = LSB
        void handle8XY7(uint8_t Vx, uint8_t Vy); // SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow
        void handle8XYE(uint8_t Vx); // SHL Vx - Shift Vx left by 1, set VF = MSB
        void handle9XY0(uint8_t Vx, uint8_t Vy); // SNE Vx, Vy - Skip next instruction if Vx != Vy
        void handleANNN(uint16_t addr); // LD I, addr - Set I = NNN
        void handleBNNN(uint16_t addr); // JP V0, addr - Jump to NNN + V0
        void handleCXNN(uint8_t Vx, uint8_t byte); // RND Vx, byte - Set Vx = random byte AND NN
        void handleDXYN(uint8_t Vx, uint8_t Vy, uint8_t nibble); // DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        void handleEX9E(uint8_t Vx); // SKP Vx - Skip next instruction if key with the value of Vx is pressed
        void handleEXA1(uint8_t Vx); // SKNP Vx - Skip next instruction if key with the value of Vx is not pressed
        void handleFX07(uint8_t Vx); // LD Vx, DT - Set Vx = delay timer value
        void handleFX0A(uint8_t Vx); // LD Vx, K - Wait for a key press, store the value of the key in Vx
        void handleFX15(uint8_t Vx); // LD DT, Vx - Set delay timer = Vx
        void handleFX18(uint8_t Vx); // LD ST, Vx - Set sound timer = Vx
        void handleFX1E(uint8_t Vx); // ADD I, Vx - Set I = I + Vx
        void handleFX29(uint8_t Vx); // LD F, Vx - Set I = location of sprite for digit Vx
        void handleFX33(uint8_t Vx); // LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2
        void handleFX55(uint8_t Vx); // LD [I], Vx - Store registers V0 through Vx in memory starting at location I
        void handleFX65(uint8_t Vx); // LD Vx, [I] - Read registers V0 through Vx from memory starting at location I

      
    protected:
    private:
        //CPU
        std::unordered_map<std::string, uint8_t> registers;
        uint16_t index, program_counter;
        uint16_t stack[16];
        uint8_t stack_ptr;

        //Timers
        uint8_t delay_t;
        uint8_t sound_t;
    
        // Memory, 4KB
        uint8_t ram[4096];

        //Graphics emulation
        PixelMap screen;

        //Keypad
        Keypad keypad;
};

#endif;