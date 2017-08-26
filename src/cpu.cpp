#include "cpu.h"
#include <string>
#include <iostream>
#include <fstream>
#include <random>
using namespace::std;

static unsigned char random_number();

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
          --sp;
          pc = stack[sp];
          stack[sp] = 0;
          pc += 2;
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

      case 0xA000: //0xANNN
        // Sets I to the address NNN
        i = opcode & 0x0FFF;
        pc += 2;
      break;

      case 0xB000: //0xBNNN
        // Jumps to the address NNN plus V0
        pc = reg[0] + (opcode & 0x0FFF);
      break;

      case 0xC000: //0xCXNN
        // Sets VX to the result of a bitwise AND operation on a random number
        // (typically 0 through 255) and NN
        reg[(opcode & 0x0F00) >> 8] = random_number() & (opcode & 0x00FF);
        pc += 2;
      break;

      case 0xD000: //0xDXYN
        // Draws a sprite at coordinate (VX, VY) that has width 8 pixels and
        // height N pixels. Each row of 8 pixels is read as bit-coded starting
        // from memory location I (I doesn't change after this). VF is set to 1
        // if any screen pixels are flipped from set to unset when the sprite
        // is drawn and 0 if it doesn't
      {
        // x and y represent coordinates. height is the height of the sprite
        // to be drawn (width is always 8)
        unsigned short x = reg[(opcode & 0x0F00) >> 8];
        unsigned short y = reg[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;

        reg[0xF] = 0; //VF set to 0 to start. will be set to 1 if a lit pixel is turned off
        for(int yline = 0; yline < height; ++yline){ // for each row...
          pixel = ram[i + yline]; // set pixel to the string of bits starting at I + row
          for(int xline = 0; xline < 8; ++xline){ // for each column...
            if((pixel & (0x80 >> xline)) != 0){ // checks one bit of pixel
              if(screen[(x + xline + ((y + yline) * 64))] == 1){
                reg[0xF] = 1;
              }
              screen[x + xline + ((y + yline) * 64)] ^= 1; //xor with 1.
            }
          }
        }
        drawflag = true;
        pc += 2;
      }
      break;

      case 0xE000:
        switch(opcode & 0x00FF){
          case 0x009E: //0xEX9E
            // Skips the next instruction if the ket stored in VX is pressed
            if(key[reg[(opcode & 0x0F00) >> 8]] != 0){
              pc += 4;
            }
            else{
              pc += 2;
            }
          break;

          case 0x00A1: //0xEXA1
            // Skips the next instruction if the key stored in VX isn't pressed
            if(key[reg[(opcode & 0x0F00) >> 8]] == 0){
              pc += 4;
            }
            else{
              pc += 2;
            }
          break;
        }
      break;

      case 0xF000:
        switch(opcode & 0x00FF){
          case 0x0007: //0xFX07
            // Sets VX to the value of the delay timer
            reg[(opcode & 0x0F00) >> 8] = delay_timer;
            pc += 2;
          break;

          case 0x000A: //0xFX0A
            // A key press is awaited, then stored in VX. All instruction is
            // halted until the next key event.
            for(int n = 0; n < 16; ++n){
              if(key[n] != 0){
                reg[(opcode & 0x0F00) >> 8] = n;
                pc += 2;
              }
            }
          break;

          case 0x0015: //0xFX15
            // Sets the delay timer to VX
            delay_timer = reg[(opcode & 0x0F00) >> 8];
            pc += 2;
          break;

          case 0x0018: //0xFX18
            // Sets the sound timer to VX
            sound_timer = reg[(opcode & 0x0F00) >> 8];
            pc += 2;
          break;

          case 0x001E: //0xFX1E
            // Adds VX to I
            i += reg[(opcode & 0x0F00) >> 8];
            pc += 2;
          break;

          case 0x0029: //0xFX29
            // Sets I to the location of the sprite for the character in VX
            i = reg[(opcode & 0x0F00) >> 8] * 0x5;
            pc += 2;
          break;

          case 0x0033: //0xFX33
            // Stores the binary coded decimal representation of VX, with the
            // most significant of three digits at the address in I, the middle
            // digit at I plys 1, and the least significant digit at I plus 2.
            ram[i]     = (reg[(opcode & 0x0F00) >> 8] / 100);
            ram[i + 1] = (reg[(opcode & 0x0F00) >> 8] / 10) % 10;
            ram[i + 2] = (reg[(opcode & 0x0F00) >> 8] % 10);
            pc += 2;
          break;

          case 0x0055: //0xFX55
            // Stores V0 through VX in memory starting at address I
            for(int n = 0; n <= ((opcode & 0x0F00) >> 8); ++n){
              ram[i + n] = reg[n];
            }

            pc += 2;
          break;

          case 0x0065: //0xFX65
            // Fills V0 through VX with values from memory starting ad address I
            for(int n = 0; n <= (opcode & 0x0F00) >> 8; ++n){
              reg[n] = ram[i + n];
            }

            pc += 2;
          break;
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

static unsigned char random_number(){
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0,255);
  return dist(rng);
}
