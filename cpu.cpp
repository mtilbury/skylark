#include "cpu.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace::std;

cpu::cpu() : opcode(0), i(0), pc(0x200), sp(0) {
  // Clear display
  clearScreen();

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

// void cpu::init(){
//   pc = 0x200; // Program counter at address 0x200
//   opcode = 0; // No current opcode
//   i = 0; // Stack register to 0
//   sp = 0; // Reset stack pointer
//
//   // Clear display
//   clearScreen();
//
//   // Clear stack
//   for(int i = 0; i < 16; ++i){
//     stack[i] = 0;
//   }
//
//   // Clear registers V0-VF
//   for(int i = 0; i < 16; ++i){
//     reg[i] = 0;
//   }
//
//   // Clear memory
//   for(int i = 0; i < 4096; ++i){
//     ram[i] = 0;
//   }
//
//   // Load fontset into memory
//   for(int i = 0; i < 80; ++i){
//     ram[i] = chip8_fontset[i];
//   }
//
//   // Clear keypad array
//   for(int i = 0; i < 16; ++i){
//     key[i] = 0;
//   }
//
//   // Reset timers
//   delay_timer = 0;
//   sound_timer = 0;
// }

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

void cpu::cycle(){
  // Obtain next opcode
  // Works by shifting the first byte to the left by adding 8 zeroes. Then,
  // by using OR, it combines both into a two byte value.
  opcode = ram[pc] << 8 | ram[pc + 1];

  // Decode the opcode
  switch(opcode & 0xF000){ // checks the first 4 bits of opcode
    // Execute with large switch statement
    case 0x0000:
      switch(opcode & 0x000F){ // checks last 4 bits of opcode
        case 0x000E: //0x00EE
          // return from subroutine
          // TODO
          break;

        case 0x0000: //0x00E0
          // clear the screen
          clearScreen();
          pc += 2;
          break;
      }

      case 0x1000: //0x1NNN
        // Jumps to address NNN
        pc = opcode & 0x0FFF;
        break;

      case 0x2000: //0x2NNN
        // Calls subroutine at NNN
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;

      case 0x3000: //0x3XNN
        // Skips the next instruction if VX = NN
        if(reg[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
          pc += 4;
        }
        else{
          pc += 2;
        }
        break;

      case 0x4000: //0x4XNN
        // Skips the next instruction if VX != NN
        if(reg[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
          pc += 4;
        }
        else{
          pc += 2;
        }
        break;

      case 0x5000: //0x5XY0
        // Skips the next instruction if VX equals VY
        if(reg[(opcode & 0x0F00) >> 8] == reg[(opcode & 0x00F0) >> 4]){
          pc += 4;
        }
        else{
          pc += 2;
        }
        break;

      case 0x6000: //0x6XNN
        // Sets VX to NN
        reg[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
        pc += 2;
        break;

      case 0x7000: //0x7XNN
        // Adds NN to VX
        reg[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
        pc += 2;
        break;

      default:
        cout << "Ruh roh! This opcode wasn't implemented!" << endl;
  }

  // Update timers
  if(delay_timer > 0){
    --delay_timer;
  }
  if(sound_timer > 0){
    if(sound_timer == 1){
      // make sound
    }
    --sound_timer;
  }
  pc += 2;
}

void cpu::clearScreen(){
  for(int i = 0; i < 64 * 32; ++i){
    screen[i] = 0;
  }
}

const unsigned short& cpu::getOpcode(){
  return opcode;
}

const unsigned char* cpu::getRegisters(){
  return reg;
}
const unsigned short& cpu::getIndex(){
  return i;
}
const unsigned short& cpu::getProgramCounter(){
  return pc;
}
const unsigned short* cpu::getStack(){
  return stack;
}
const unsigned short& cpu::getStackPointer(){
  return sp;
}
