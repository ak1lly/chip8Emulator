#include <iostream>
#include "Keypad.h"

uint8_t SDL_Keymap[NUM_KEYS] = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
    };

Keypad::Keypad(){
    memset(keypad, 0, sizeof(keypad));
}

void Keypad::handle_key_down(uint8_t eventKey){
    for (int i=0; i < sizeof(SDL_Keymap); i++){
        if(eventKey == SDL_Keymap[i]){
            keypad[i] = 1;
            std::cout<<"Pressed key: "<<i<<"ARRAY LOGGING"<<keypad<<std::endl;
        }
    }
}

void Keypad::handle_key_up(uint8_t eventKey){
    for (int i=0; i < sizeof(SDL_Keymap); i++){
        if(eventKey == SDL_Keymap[i]){
            keypad[i] = 0;
            std::cout<<"Released key: "<<i<<"ARRAY LOGGING"<<keypad<<std::endl;
        }
    }
}

