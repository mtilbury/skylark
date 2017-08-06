/*
 *  cpu.h
 *
 *  Represents the internals of the CHIP-8 including the cpu and memory
 *
 */

unsigned short opcode; // holds the current 2-byte opcode
unsigned char ram[4096]; // represents the 4096 8-bit memory locations
unsigned char reg[16]; // represents the CPU registers V0 through VE
unsigned char fontset[80] // the fontset for the CHIP-8 stored in memory

unsigned short I; // index register
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
bool drawflag = false; // if the drawflag is set to true, the screen is drawn

// Skylark Functions
void init(); // initializes the cpu
void cycle(); // completes one cycle of emulation
void loadGame(string game); // loads the game
void setKeys(); // sets the key press state
