#include "cpu.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace::std;

void cpu::init(){
  pc = 0x200; // Program counter at address 0x200
  opcode = 0; // No current opcode
  i = 0; // Stack register to 0
  sp = 0; // Reset stack pointer

  // Clear display
  for(int i = 0; i < 64 * 32; ++i){
    screen[i] = 0;
  }

  // Clear stack
  for(int i = 0; i < 16; ++i){
    stack[i] = 0;
  }

  // Clear registers V0-VF
  for(int i = 0; i < 16; ++i){
    reg[i] = 0;
  }

  // Clear memory
  for(int i = 0; i < 4096; ++i){
    ram[i] = 0;
  }

  // Load fontset into memory
  for(int i = 0; i < 80; ++i){
    ram[i] = chip8_fontset[i];
  }

  // Clear keypad array
  for(int i = 0; i < 16; ++i){
    key[i] = 0;
  }

  // Reset timers
  delay_timer = 0;
  sound_timer = 0;
}

void cpu::loadGame(istream &game){
  // get length of file
  game.seekg(0, game.end);
  int length = game.tellg();
  game.seekg(0, game.beg);

  // Create dynamic array for size of file. Reads one byte at a time to buffer
  char * buffer = new char [length];
  game.read(buffer, length);

  // Set buffer into memory starting at position 512 (0x200)
  for(int i = 0; i < length; ++i){
    ram[i + 512] = buffer[i];
  }

  // Free dynamically allocated memory
  delete[] buffer;
}
