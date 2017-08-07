#include "cpu.h"

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
