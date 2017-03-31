#include <iostream>
#include <exception>
#include <memory>
#include <SFML/Graphics.hpp>
#include "character.h"
using namespace std;

int main( int argc, char* argv[] )
{
  Character c( "ryu", 1 );

  sf::RenderWindow window( sf::VideoMode( 800, 600 ), "My Window" );
  std::shared_ptr<sf::Texture> tex = c.get_texture();
  sf::Sprite s;
  s.setTexture( *tex );
  while ( window.isOpen() )
  {
    sf::Event event;
    while ( window.pollEvent( event ) )
    {
      if ( event.type == sf::Event::Closed )
        window.close();
    }

    window.clear();
    window.draw( s );
    window.display();
  }

  return 0;
}
