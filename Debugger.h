#include <iostream>
#include <fstream>
#include <iomanip>

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

private:
  cpu chip8;
};

void Debugger::cycle(){
  chip8.cycle();
}

void Debugger::loadGame(std::istream &game){
  chip8.loadGame(game);
}

void Debugger::printOpcode(){
  const unsigned short oc = chip8.getOpcode();
  std::cout << "Opcode: 0x" << std::hex << oc << std::endl;
}
void Debugger::printRegisters(){

}
void Debugger::printIndex(){

}
void Debugger::printProgramCounter(){

}
void Debugger::printStack(){

}
void Debugger::printStackPointer(){

}
