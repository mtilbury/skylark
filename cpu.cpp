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
    break;

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

      case 0x8000:
        switch(opcode & 0x000F){
          case 0x0000: //0x8XY0
            // Sets VX to the value of VY
            reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x00F0) >> 4];
            pc += 2;
          break;

          case 0x0001: //0x8XY1
            // Sets VX to VX or VY
            reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x0F00) >> 8] | reg[(opcode & 0x00F0) >> 4];
            pc += 2;
          break;

          case 0x0002: //0x8XY2
            // Sets VX to VX and VY
            reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x0F00) >> 8] & reg[(opcode & 0x00F0) >> 4];
            pc += 2;
          break;

          case 0x0003: //0x8XY3
            // Sets VX to VX xor VY
            reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x0F00) >> 8] ^ reg[(opcode & 0x00F0) >> 4];
            pc += 2;
          break;

          case 0x0004: //0x8XY4
            // Adds VY to VX. VF set to 1 when there's a carry and to 0 when there isn't
            // Explicit cast to int to tell if there's overflow
            if((int) reg[(opcode & 0x0F00) >> 8] + (int) reg[(opcode & 0x00F0) >> 4] > 255){
              reg[0xF] = 1;
            }
            else{
              reg[0xF] = 0;
            }
            reg[(opcode & 0x0F00) >>8] += reg[(opcode & 0x00F0) >> 4];
            pc += 2;
          break;

          case 0x0005: //0x8XY5
            // Subtracts VY from VX. VF is set to 0 when there's a borrow, 1 otherwise
            if((int) reg[(opcode & 0x0F00) >> 8] - (int) reg[(opcode & 0x00F0) >> 4] < 0){
              reg[0xF] = 0;
            }
            else{
              reg[0xF] = 1;
            }
            reg[(opcode & 0x0F00) >> 8] -= reg[(opcode & 0x00F0) >> 4];
            pc += 2;
          break;

          case 0x0006: //0x8XY6
            // Shifts VX right by one. VF set to least significant bit before shift
            reg[0xF] = reg[(opcode & 0x0F00) >> 8] & 0x1;
            reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x0F00) >> 8] >> 1;
            pc += 2;
          break;

          case 0x0007: //0x8XY7
            // Sets VX to VY - VX. If there's a borrow, VF set to 0. 1 otherwise.
            if((int) reg[(opcode & 0x00F0) >> 4] - (int) reg[(opcode & 0x0F00) >> 8] < 0){
              reg[0xF] = 0;
            }
            else{
              reg[0xF] = 1;
            }
            reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x00F0) >> 4] -reg[(opcode & 0x0F00) >> 8];
            pc += 2;
          break;

          case 0x000E: //0x8XYE
            // Shifts VX left by one. VF is set to the value of the most significant bit of VX
            reg[0xF] = reg[(opcode & 0x0F00) >> 8] & 0x80;
            reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x0F00) >> 8] << 1;
            pc += 2;
          break;
        }
      break;

      case 0x9000: //0x9XY0
        // Skips next instruction if VX doesn't equal VY
        if(reg[(opcode & 0x0F00) >> 8] != reg[(opcode & 0x00F0) >> 4]){
          pc += 4;
        }
        else{
          pc += 2;
        }
      break;

      default:
        cout << "Ruh roh! This opcode wasn't implemented!" << endl;
        pc += 2;
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
