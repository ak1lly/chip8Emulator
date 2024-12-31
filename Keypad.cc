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
    

const int Keypad::operator[](int i){
    return keypad[i];
}

Keypad::Keypad(){
    memset(keypad, 0, sizeof(keypad));
}

void Keypad::handle_key_down(uint8_t eventKey){
    for (int i=0; i < sizeof(SDL_Keymap); i++){
        if(eventKey == SDL_Keymap[i]){
            keypad[i] = 1;
            std::cout<<"Pressed key: "<<i<<std::endl;
            print_array(keypad, 16);
        }
    }
}

void Keypad::handle_key_up(uint8_t eventKey){
    for (int i=0; i < sizeof(SDL_Keymap); i++){
        if(eventKey == SDL_Keymap[i]){
            keypad[i] = 0;
            std::cout<<"Released key: "<<i<<std::endl;
            print_array(keypad, 16);
        }
    }
}

void Keypad::print_array(const int arr[], int size){
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}


