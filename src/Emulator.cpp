#include <iostream>

#include "CPU.h"
#include "Emulator.h"
#include "SDL2/SDL.h"

using namespace std;

Emulator::Emulator() : cpu(CPU()), gameOn(false),
// Graphics initialization
keymap{
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
},
SCREEN_WIDTH(512), SCREEN_HEIGHT(256), 
window(nullptr), renderer(nullptr), texture(nullptr), pixel_buffer{0}
{
    // Set up graphics variables
    window = SDL_CreateWindow( "CHIP-8 Skylark", SDL_WINDOWPOS_UNDEFINED,
                      SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                      SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    
    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

void Emulator::cycle(){
    cpu.cycle();
}

void Emulator::loadGame(istream &game){
    cpu.loadMem(game);
}

void Emulator::run(){
    gameOn = true;
    while(gameOn){
        this->cycle();
        // cout << "Opcode: 0x" << hex << cpu.getOpcode() <<
        //     " pc = 0x" << hex << cpu.getProgramCounter() << endl;
        if(cpu.drawflag){
            this->draw();
        }
        this->checkKeyPress();
        cpu.drawflag = false;
        SDL_Delay(2);
    }
}

void Emulator::draw(){
    for(int i = 0; i < 2048; ++i){
        if(cpu.screen[i] == 0){ // if the pixel is off, set it to black
            pixel_buffer[i] = 0xFF000000;
        }
        else{ // if it's on, set it to white
            pixel_buffer[i] = 0xFFFFFFFF;
        }
    }
    // Update the texture
    SDL_UpdateTexture(texture, NULL, pixel_buffer, 64 * sizeof(unsigned int));

    // Clear screen
    SDL_RenderClear(renderer); // clears the screen
    SDL_RenderCopy(renderer, texture, NULL, NULL); // copy texture to rendering target
    SDL_RenderPresent(renderer); // updates the screen with new rendering
}

void Emulator::checkKeyPress(){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);

        // Process keydown events
        if (e.type == SDL_KEYDOWN) {
            for (int i = 0; i < 16; ++i) {
                if (e.key.keysym.sym == keymap[i]) {
                    cpu.key[i] = 1;
                }
            }
        }
        // Process keyup events
        if (e.type == SDL_KEYUP) {
            for (int i = 0; i < 16; ++i) {
                if (e.key.keysym.sym == keymap[i]) {
                    cpu.key[i] = 0;
                }
            }
        }
    }
}