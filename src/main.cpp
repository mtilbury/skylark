#include <iostream> // for input/output to terminal
#include <fstream> // to open and read from ROM file

#include "Emulator.h"
#include "SDL2/SDL.h"

using namespace std;

int main(int argc, char* argv[]){

  // Makes sure there's a proper number of arguments
  if(argc != 2){
    cout << "USAGE: skylark.exe <ROM_FILENAME>" << endl;
    exit(EXIT_FAILURE);
  }
  // Initialize the emulator
  Emulator emulator;
  string game(argv[1]);

  // Load ROM file
  ifstream is(game, ifstream::binary);
  if(is.is_open()){
    emulator.loadGame(is);
  }
  else{
    cerr << "Not a valid file." << endl;
    return 1;
  }

  emulator.run();

  return 0;
}
