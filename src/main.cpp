#include "CPU.h"
#include <iostream> // for input/output to terminal
#include <fstream> // to open and read from ROM file
#include "SDL2/SDL.h"

using namespace std;

int main(int argc, char* argv[]){

  // Makes sure there's a proper number of arguments
  if(argc != 2){
    cout << "USAGE: skylark.exe <ROM_FILENAME>" << endl;
    exit(EXIT_FAILURE);
  }
  // Initialize the emulator
  CPU skylark;
  string game(argv[1]);

  // Load ROM file
  ifstream is(game, ifstream::binary);
  if(is.is_open()){
    skylark.loadGame(is);
  }
  else{
    cout << "Not a valid file." << endl;
    return 0;
  }

  // Set up input
  uint8_t keymap[16] = {
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
  // Set up graphics
  // Size of window to be created
  const int SCREEN_WIDTH = 512;
  const int SCREEN_HEIGHT = 256;

  // Initialize and define window. Window position is undefined
  SDL_Window* window = NULL;
  window = SDL_CreateWindow( "CHIP-8 Skylark", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

  // Creates the renderer object
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  // Creates the texture object
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING, 64, 32);

  // Screen buffer
  unsigned int pixel_buffer[64 * 32];

  // Loop variable
  bool gameOn = true;

  while(gameOn){
    // Emulate one cycle
    skylark.cycle();
    cout << "Opcode: 0x" << hex << skylark.getOpcode() <<
            " pc = 0x" << hex << skylark.getProgramCounter() << endl;

    // If the draw flag is set, update the screen
    if(skylark.drawflag){
      // draw graphics
      for(int i = 0; i < 2048; ++i){
        if(skylark.screen[i] == 0){ // if the pixel is off, set it to black
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

    // Process SDL events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);

        // Process keydown events
        if (e.type == SDL_KEYDOWN) {
            for (int i = 0; i < 16; ++i) {
                if (e.key.keysym.sym == keymap[i]) {
                    skylark.key[i] = 1;
                }
            }
        }
        // Process keyup events
        if (e.type == SDL_KEYUP) {
            for (int i = 0; i < 16; ++i) {
                if (e.key.keysym.sym == keymap[i]) {
                    skylark.key[i] = 0;
                }
            }
        }
    }
    skylark.drawflag = false;
    SDL_Delay(2);
  }


  return 0;
}
