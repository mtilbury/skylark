#include "cpu.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
  cpu skylark;
  skylark.init();

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

  for(;;){
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
