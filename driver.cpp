#include <iostream>
#include <SFML/Graphics.hpp>
#include "main_menu.h"
int main( int argc, char* argv[] )
{
  //  Initialize window settings
  //////////////////////////////////////////////////////////////////////////////
  int window_width = 1000;
  int window_height = 626;
  sf::RenderWindow* window;
  window = new sf::RenderWindow( sf::VideoMode( window_width, window_height ),
                                 "My Window" );
  window->setPosition( sf::Vector2i(
      ( sf::VideoMode::getDesktopMode().width / 2 ) - ( window_width / 2 ),
      ( sf::VideoMode::getDesktopMode().height / 2 ) -
          ( window_height / 2 ) ) );
  window->setVerticalSyncEnabled( true );
  /****************************************************************************/

  //  Create textures that need to live for the life of the program
  //////////////////////////////////////////////////////////////////////////////
  sf::Texture* background = new sf::Texture;
  /****************************************************************************/

  // create the menu object and pass it the window and texture addresses
  Main_menu* menu = new Main_menu( window, background );

  while ( window->isOpen() )
  {
    int selection = 0;
    while ( !( selection ) && window->isOpen() )
    {
      selection = menu->run();
      // find out which button was selected
      if ( selection == 1 )
        std::cout << "Server\n";
      else if ( selection == 2 )
        std::cout << "Client\n";
    }
  }

  // delete jump;

  return 0;
}
