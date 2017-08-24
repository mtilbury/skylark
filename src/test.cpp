#include "cpu.h"
#include "Debugger.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]){


  if(argc != 2){
    cout << "USAGE: test.exe ROM_FILENAME" << endl;
    exit(EXIT_FAILURE);
  }

  Debugger debug;
  string game(argv[1]);

  ifstream is(game, ifstream::binary);
  if(is.is_open()){
    debug.loadGame(is);
  }
  else{
    cout << "that's not a file" << endl;
    return 0;
  }

  cout << "Press enter to step through emulation. Type 'h' for other commands." << endl;

  string input;
  while(getline(cin, input)){
    if(input.empty()){
      debug.cycle();
      debug.printOpcode();
      debug.printDebug();
    }

    else if(input == "reg"){
      debug.printRegisters();
    }

    else if(input == "i"){
      debug.printIndex();
    }

    else if(input == "pc"){
      debug.printProgramCounter();
    }

    else if(input == "stack"){
      debug.printStack();
    }

    else if(input == "sp"){
      debug.printStackPointer();
    }

    else if(input == "q"){
      return 0;
    }

    else if(input == "h"){
      cout << "  reg: Prints registers V0-VF" << endl;
      cout << "    i: Prints the current index" << endl;
      cout << "   pc: Prints the program counter" << endl;
      cout << "stack: List contents of the stack" << endl;
      cout << "   sp: Prints the stack pointer" << endl;
      cout << "    q: Exits the debugger" << endl;
    }

    else{
      cout << "Unrecognized command. Type 'h' for more information." << endl;
    }
  }

  debug.cycle();
  debug.printOpcode();
  debug.cycle();
  debug.printOpcode();

  cout << "if this prints that means it actually compiled congrats" << endl;
}
