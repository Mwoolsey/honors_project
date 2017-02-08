#include "main_menu.h"
#include <stdexcept>

Main_menu::Main_menu( sf::RenderWindow* window, sf::Texture* bg )
    : _window( window ), _background_texture( bg )
{
  if ( !_background_image.loadFromFile( "assets/main_menu_bg.png" ) )
    throw std::runtime_error( "Error loading main_menu_bg.png" );
}

void Main_menu::run( void )
{
  if ( !_background_texture->loadFromImage( _background_image ) )
    throw std::runtime_error( "Error loading main_menu_bg.png" );

  _main_menu_bg.setTexture( *_background_texture );

  sf::Event event;

  while ( _window->pollEvent( event ) )
  {
    switch ( event.type )
    {
      case sf::Event::Closed:
      {
        _window->close();
        break;
      }
      default:
        break;
    }
  }
  _window->clear();
  _window->draw( _main_menu_bg );
  _window->display();
}
