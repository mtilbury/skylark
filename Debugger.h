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

      default:
        debug = "n/a";
  }
}
