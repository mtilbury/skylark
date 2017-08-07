/*
 *  cpu.h
 *
 *  Represents the internals of the CHIP-8 including the cpu and memory
 *
 */

#include<string>

class cpu {
public:
  bool drawflag = false; // if the drawflag is set to true, the screen is drawn
  void init(); // initializes the cpu
  void cycle(); // completes one cycle of emulation
  void loadGame(std::istream &game); // loads the game
  void setKeys(); // sets the key press state
  unsigned char ram[4096]; // represents the 4096 8-bit memory locations

private:
  unsigned short opcode; // holds the current 2-byte opcode

  unsigned char reg[16]; // represents the CPU registers V0 through VE
  unsigned char fontset[80]; // the fontset for the CHIP-8 stored in memory

  unsigned short i; // index register
  unsigned short pc; // program counter
  unsigned char screen[64 * 32]; // represents the 64x32 pixel b/w screen

  // Timers count 60 times per second and count down to 0
  unsigned char delay_timer;
  unsigned char sound_timer;

  // The stack holds the program counter before a jump in order to return to it
  // after. There are 16 levels in the stack.
  unsigned short stack[16];
  unsigned short sp; // stack pointer stores the current stack level

  unsigned char key[16]; // used for keypad control

  // Defines the fontset
  unsigned char chip8_fontset[80] =
  {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
};
