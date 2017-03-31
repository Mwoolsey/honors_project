#include <iostream>
#include "character.h"
#include <string>
using namespace std;

int main( int argc, char* argv[] )
{

  std::cerr << "Creating character\n";
  Character c( "ryu", 1 );
  std::cerr << "Character created\n";

  return 0;
}
