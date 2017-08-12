#include "cpu.h"
#include "Debugger.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]){
  Debugger debug;
  string game(argv[1]);

  if(argc != 2){
    cout << "USAGE: test.exe ROM_FILENAME" << endl;
    exit(EXIT_FAILURE);
  }

  ifstream is(game, ifstream::binary);
  if(is.is_open()){
    debug.loadGame(is);
  }
  else{
    cout << "that's not a file" << endl;
    return 0;
  }

  debug.cycle();
  debug.printOpcode();
  debug.cycle();
  debug.printOpcode();

  cout << "if this prints that means it actually compiled congrats" << endl;
}
