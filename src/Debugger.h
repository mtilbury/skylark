#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

class Debugger {
public:
  void cycle();
  void loadGame(std::istream &game);
  void printOpcode();
  void printRegisters();
  void printIndex();
  void printProgramCounter();
  void printStack();
  void printStackPointer();
  void printDebug();

private:
  cpu chip8;
  void printOneRegister(unsigned char regIndex, std::ostream& out);
  void printOneStack(unsigned short stackIndex, std::ostream& out);
  void updateDebugInfo();
  std::string debug; // holds debug information for latest instruction
};

// Completes one cycle of emulation for the internal cpu
void Debugger::cycle(){
  chip8.cycle();
  updateDebugInfo();
}

// Loads a game to the internal cpu
void Debugger::loadGame(std::istream &game){
  chip8.loadGame(game);
}

// Prints the last opcode held by the cpu
void Debugger::printOpcode(){
  const unsigned short oc = chip8.getOpcode();
  std::cout << "Opcode: 0x" << std::hex << oc << std::endl;
}

// Prints the registers of the cpu
void Debugger::printRegisters(){
  const unsigned char* V = chip8.getRegisters();
  std::cout << "V0 V1 V2 V3 V4 V5 V6 V7 V8 V9 VA VB VC VD VE VF" << std::endl;
  for(int i = 0; i < 16; ++i){
    printOneRegister(V[i], std::cout);
    if(i < 15) std::cout << " ";
    else std::cout << std::endl;
  }
}

// Prints the index register of the cpu
void Debugger::printIndex(){
  std::cout << "i = 0x" << std::hex << chip8.getIndex() << std::endl;
}

// Prints the program counter of the cpu (the location in memory of the
// next opcode)
void Debugger::printProgramCounter(){
  std::cout << "pc = 0x" << std::hex << chip8.getProgramCounter() << std::endl;
}

// Prints the contents of the stack in four rows of four
void Debugger::printStack(){
  const unsigned short* stack = chip8.getStack();
  for(int i = 0; i < 16; ++i){
    printOneStack(stack[i], std::cout);
    if((i + 1) % 4 == 0) std::cout << std::endl;
    else std::cout << " ";
  }
}

// Prints the stack pointer (how "deep" the stack currently is)
void Debugger::printStackPointer(){
  std::cout << "sp = " << std::hex << chip8.getStackPointer() << std::endl;
}

// Prints debug info from the last cpu instruction
void Debugger::printDebug(){
  std::cout << "DEBUG: " << debug << std::endl;
}

// Prints a single register using hex formatting
void Debugger::printOneRegister(unsigned char regIndex, std::ostream& out){
  if((int) regIndex < 0x10){
    out << "0" << std::hex << (int) regIndex;
  }
  else{
    out << std::hex << (int) regIndex;
  }
}

// Prints a single stack entry with hex formatting
void Debugger::printOneStack(unsigned short stackIndex, std::ostream& out){
  if(stackIndex < 0x0010){
    out << "000" << stackIndex;
  }
  else if(stackIndex < 0x0100){
    out << "00" << stackIndex;
  }
  else if(stackIndex < 0x1000){
    out << "0" << stackIndex;
  }
  else{
    out << stackIndex;
  }
}

void Debugger::updateDebugInfo(){
  const unsigned short opcode = chip8.getOpcode();
  const unsigned char* reg = chip8.getRegisters();
  std::ostringstream ss;
  std::string vx, vy;
  switch(opcode & 0xF000){ // checks the first 4 bits of opcode
    // Execute with large switch statement
    case 0x0000:
      switch(opcode & 0x000F){ // checks last 4 bits of opcode
        case 0x000E: //0x00EE
          debug = "Return from subroutine.";
        break;

        case 0x0000: //0x00E0
          debug = "Cleared screen.";
        break;
      }
    break;

      case 0x1000: //0x1NNN
        ss << "Jump to address 0x" << std::hex << (opcode & 0x0FFF);
        debug = ss.str();
      break;

      case 0x2000: //0x2NNN
        ss << "Call to subroutine at address 0x" << std::hex << (opcode & 0x0FFF);
        debug = ss.str();
      break;

      case 0x3000: //0x3XNN
        // Skips the next instruction if VX = NN
        if(reg[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
          ss << "Skip the next instruction because V" << std::hex << ((opcode & 0x0F00) >> 8)
             << " = 0x" << std::hex << (opcode & 0x00FF);
          debug = ss.str();
        }
        else{
          ss << "Doesn't skip the next instruction because V" << std::hex << ((opcode & 0x0F00) >> 8)
             << " != 0x" << std::hex << (opcode & 0x00FF);
          debug = ss.str();
        }
      break;

      case 0x4000: //0x4XNN
        // Skips the next instruction if VX != NN
        if(reg[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
          ss << "Skip the next instruction because V" << std::hex << ((opcode & 0x0F00) >> 8)
             << " != 0x" << std::hex << (opcode & 0x00FF);
          debug = ss.str();
        }
        else{
          ss << "Doesn't skip the next instruction because V" << std::hex << ((opcode & 0x0F00) >> 8)
             << " = 0x" << std::hex << (opcode & 0x00FF);
          debug = ss.str();
        }
      break;

      case 0x5000: //0x5XY0
        // Skips the next instruction if VX equals VY
        if(reg[(opcode & 0x0F00) >> 8] == reg[(opcode & 0x00F0) >> 4]){
          ss << "Skip the next instruction because V" << std::hex << ((opcode & 0x0F00) >> 8)
             << " = V" << std::hex << ((opcode & 0x00F0) >> 4);
          debug = ss.str();
        }
        else{
          ss << "Doesn't skip the next instruction because V" << std::hex << ((opcode & 0x0F00) >> 8)
             << " = V" << std::hex << ((opcode & 0x00F0) >> 4);
          debug = ss.str();
        }
      break;

      case 0x6000: //0x6XNN
        ss << "Set V" << ((opcode & 0x0F00) >> 8) << " to 0x" << std::hex << (opcode & 0x00FF);
        debug = ss.str();
      break;

      case 0x7000: //0x7XNN
        ss << "Adds 0x" << std::hex << (opcode & 0x00FF) << " to V" << std::hex << ((opcode & 0x0F00) >> 8);
        debug = ss.str();
      break;

      case 0x8000:
      ss << "V" << std::hex << ((opcode & 0x0F00) >> 8);
      vx = ss.str();
      ss.str("");

      ss << "V" << std::hex << ((opcode & 0x00F0) >> 4);
      vy = ss.str();
      ss.str("");

        switch(opcode & 0x000F){
          case 0x0000: //0x8XY0
            ss << "Set " << vx << " to the value of " << vy;
            debug = ss.str();
          break;

          case 0x0001: //0x8XY1
            ss << "Set " << vx << " to " << vx << " OR " << vy;
            debug = ss.str();
          break;

          case 0x0002: //0x8XY2
            ss << "Set " << vx << " to " << vx << " AND " << vy;
            debug = ss.str();
          break;

          case 0x0003: //0x8XY3
            ss << "Set " << vx << " to " << vx << " XOR " << vy;
            debug = ss.str();
          break;

          case 0x0004: //0x8XY4
            ss << "Adds " << vy << " to " << vx << ". VF is set to 0 if there's a carry, 1 if not.";
            debug = ss.str();
          break;

          case 0x0005: //0x8XY5
            ss << "Subtracts " << vy << " from " << vx << ". VF is set to 0 if there's a borrow, 1 if not.";
            debug = ss.str();
          break;

          case 0x0006: //0x8XY6
            ss << "Shifts " << vx << " right by one bit. VF is set to the most significant bit before the shift.";
            debug = ss.str();
          break;

          case 0x0007: //0x8XY7
            ss << "Sets " << vx << " to " << vy << " minus " << vx << ". VF is set to 0 if there's a borrow, 1 if not.";
            debug = ss.str();
          break;

          case 0x000E: //0x8XYE
            ss << "Shifts " << vx << " left by one bit. VF is set to the most significant bit before the shift.";
            debug = ss.str();
          break;
        }
      break;

      case 0x9000: //0x9XY0
        ss << "Skips the next instruction if V" << std::hex << ((opcode & 0x0F00) >> 8)
           << " doesn't equal V" << std::hex << ((opcode & 0x00F0) >> 4);
        debug = ss.str();
      break;

      case 0xA000: //0xANNN
        ss << "Sets I to the address 0x" << std::hex << (opcode & 0x0FFF);
        debug = ss.str();
      break;

      case 0xB000: //0xBNNN
        ss << "Jump to the address 0x" << std::hex << (opcode & 0x0FFF) << " plus V0";
        debug = ss.str();
      break;

      case 0xC000: //0xCXNN
        ss << "Set V" << std::hex << ((opcode & 0x0F00) >> 8) << " to a random number between 0 and 255 and "
           << std::hex << (opcode & 0x00FF);
        debug = ss.str();
      break;

      case 0xD000: //0xDXYN
        debug = "Draws sprite";
      break;

      case 0xE000:
        switch(opcode & 0x00FF){
          case 0x009E: //0xEX9E
            ss << "Skips the next instruction if the key stored in V" << std::hex
               << ((opcode & 0x0F00) >> 8) << " is pressed";
            debug = ss.str();
          break;

          case 0x00A1: //0xEXA1
            ss << "Skips the next instruction if the key stored in V" << std::hex
               << ((opcode & 0x0F00) >> 8) << " isn't pressed";
            debug = ss.str();
          break;
        }
      break;

      case 0xF000:
        ss << "V" << std::hex << ((opcode & 0x0F00) >> 8);
        vx = ss.str();
        ss.str("");

        switch(opcode & 0x00FF){
          case 0x0007: //0xFX07
            ss << "Sets " << vx << " to the value of the delay timer";
            debug = ss.str();
          break;

          case 0x000A: //0xFX0A
            ss << "Awaiting key press to store in " << vx;
            debug = ss.str();
          break;

          case 0x0015: //0xFX15
            ss << "Sets the delay timer to " << vx;
            debug = ss.str();
          break;

          case 0x0018: //0xFX18
            ss << "Sets the sound timer to " << vx;
            debug = ss.str();
          break;

          case 0x001E: //0xFX1E
            ss << "Adds " << vx << " to I";
            debug = ss.str();
          break;

          case 0x0029: //0xFX29
            ss << "Sets I to the location of the sprite for the character in " << vx;
            debug = ss.str();
          break;

          case 0x0033: //0xFX33
            ss << "Stores the binary coded decimal represenation of VX in memory."
               << " The hundreds digit at I, tens at I+1, and the ones at I+2";
            debug = ss.str();
          break;

          case 0x0055: //0xFX55
            ss << "Stores V0 to " << vx << " in memory starting at address I";
            debug = ss.str();
          break;

          case 0x0065: //0xFX65
            ss << "Fills V0 to " << vx << " with values from memory starting at address I";
            debug = ss.str();
          break;
        }
      break;

      default:
        debug = "n/a";
  }
}
