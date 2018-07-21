#ifndef SKYLARK_EMULATOR_H_
#define SKYLARK_EMULATOR_H_

/* The Emulator class contains a CHIP-8 CPU object, and has methods
 * to cycle the CPU and draw graphics to the screen
 */

#include <string>
#include "CPU.h"
#include "SDL2/SDL.h"

class Emulator{
public:
    Emulator();
    virtual void cycle(); // completes one cycle of emulation
    virtual void loadGame(std::istream &game); // loads the game from input stream
    virtual void run(); // run emulation loop
private:
    CPU cpu;
    bool gameOn;
    void draw();
    void checkKeyPress();

    // Graphics 
    const uint8_t keymap[16];
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    unsigned int pixel_buffer[64 * 32];
};

#endif  // SKYLARK_EMULATOR_H_