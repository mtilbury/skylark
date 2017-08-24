#include "cpu.h"
#include <iostream> // for input/output to terminal
#include <fstream> // to open and read from ROM file

using namespace std;

int main(){
  // Initialize the emulator
  cpu skylark;
  skylark.init();

  // Load ROM file
  cout << "Please enter a ROM file." << endl;
  string game;
  cin >> game;
  ifstream is(game, ifstream::binary);
  if(is.is_open()){
    skylark.loadGame(is);
  }
  else{
    cout << "Not a valid file." << endl;
    return 0;
  }

  // Set up graphics
  // Set up input
  for(int i = 0; i < 4096; ++i){
    cout << hex << static_cast<int>(skylark.ram[i]) << " ";
  }

  for(int i = 0; i < 20; ++i){
    // Emulate one cycle
    skylark.cycle();

    // If the draw flag is set, update the screen
    if(skylark.drawflag){
      // draw graphics
    }
    // skylark.setKeys();
  }
  return 0;
}
