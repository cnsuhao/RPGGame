#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char* argv[]){
  SDL_Init(SDL_INIT_EVERYTHING);
  int a = 5;
  cout << "The program was run with the following command:\n\t" << argv[0] << "\n";
  cout << "5 + N[arguments] = " << a + argc << "\n";
  SDL_Quit();
  return 0;
}

