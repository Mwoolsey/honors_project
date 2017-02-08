#include <iostream>
#include <SFML/Graphics.hpp>
#include "main_menu.h"
int main( int argc, char* argv[] )
{
  //
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
  //////////////////////////////////////////////////////////////////////////////

  //
  //  Create textures that need to live for the life of the program
  //////////////////////////////////////////////////////////////////////////////

  // texture for the window background
  sf::Texture* background = new sf::Texture;
  // create the menu object and pass it the texture addresses
  Main_menu* menu = new Main_menu( window, background );
  // execute the main menu section

  //////////////////////////////////////////////////////////////////////////////

  sf::Sprite BG;
  BG.setTexture( *background );

  sf::Texture texture2;
  if ( !texture2.loadFromFile( "assets/idle1.png" ) )
    std::cerr << "Couldn't load texture2 from file\n";

  sf::Sprite dualWield( texture2 );
  dualWield.setPosition(
      sf::Vector2f( ( window_width / 2 ) - ( texture2.getSize().x / 2 ),
                    window_height / 2 ) );
  while ( window->isOpen() )
  {
    // start by bringing up the main menu
    menu->run();

    // need to see any events that were triggered since last iteration
    // sf::Event event;

    /*
      while ( window->pollEvent( event ) )
      {
        switch ( event.type )
        {
          case sf::Event::Closed:
          {
            window->close();
            break;
          }
          case sf::Event::MouseButtonReleased:
          {
            std::cout << "You released the button\n";
            break;
          }
          case sf::Event::MouseButtonPressed:
          {
            std::cout << "You clicked a button\n";
            break;
          }
          case sf::Event::KeyPressed:
          {
            if ( event.key.code == sf::Keyboard::Space )
            {
              // texture2.loadFromFile( "DW_jump.png" );
              sf::Image image;
              image.loadFromFile( "assets/DW_jump.png" );
              texture2.loadFromImage( image );
            }
            break;
          }
          default:
            break;
        }
      }

    dualWield.setTexture( texture2 );
    */
    window->clear();
    // window->draw( BG );
    // window->draw( dualWield );
    // window->display();
  }

  // delete jump;

  return 0;
}
