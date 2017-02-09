#include "main_menu.h"
#include <stdexcept>

Main_menu::Main_menu( sf::RenderWindow* window, sf::Texture* bg )
    : _window( window ), _background_texture( bg )
{
  int window_width = _window->getSize().x;
  int window_height = _window->getSize().y;
  if ( !_background_image.loadFromFile( "assets/main_menu_bg.png" ) )
    throw std::runtime_error( "Error loading main_menu_bg.png" );
  if ( !_start_button_up_texture.loadFromFile( "assets/StartButtonUp.png" ) )
    throw std::runtime_error( "Error loading StartButtonUp.png" );
  if ( !_start_button_down_texture.loadFromFile(
            "assets/StartButtonDown.png" ) )
    throw std::runtime_error( "Error loading StartButtonDown.png" );
  if ( !_join_button_up_texture.loadFromFile( "assets/JoinButtonUp.png" ) )
    throw std::runtime_error( "Error loading JoinButtonUp.png" );
  if ( !_join_button_down_texture.loadFromFile( "assets/JoinButtonDown.png" ) )
    throw std::runtime_error( "Error loading JoinButtonDown.png" );

  _start_button_sprite.setPosition( sf::Vector2f(
      ( ( window_width / 2 ) - ( _start_button_up_texture.getSize().x / 2 ) ),
      window_height / 3 ) );

  _join_button_sprite.setPosition( sf::Vector2f(
      ( ( window_width / 2 ) - ( _join_button_up_texture.getSize().x / 2 ) ),
      window_height / 2 ) );
}

int Main_menu::run( void )
{
  if ( !_background_texture->loadFromImage( _background_image ) )
    throw std::runtime_error( "Error loading main_menu_bg.png" );

  _main_menu_bg.setTexture( *_background_texture );

  // get position of mouse relative to the window
  sf::Vector2i position = sf::Mouse::getPosition( *_window );

  event_handler( handle_button_animation( position ) );

  _window->clear();

  // Draw everything to the window
  _window->draw( _main_menu_bg );
  _window->draw( _start_button_sprite );
  _window->draw( _join_button_sprite );
  _window->display();
}

int Main_menu::handle_button_animation( sf::Vector2i& position )
{
  if ( ( position.x > 410 && position.x < 590 ) &&
       ( position.y > 210 && position.y < 250 ) )
  {
    _start_button_sprite.setTexture( _start_button_down_texture );
    return 1;
  }
  if ( ( position.x > 410 && position.x < 590 ) &&
       ( position.y > 315 && position.y < 360 ) )
  {
    _join_button_sprite.setTexture( _join_button_down_texture );
    return 2;
  }
  _join_button_sprite.setTexture( _join_button_up_texture );
  _start_button_sprite.setTexture( _start_button_up_texture );
  return 0;
}

int Main_menu::event_handler( int button_hovered )
{
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
      case sf::Event::MouseButtonReleased:
      {
        if ( button_hovered == 0 )
        {
          return 0;
          break;
        }
        else if ( button_hovered == 1 )
        {
          _window->close();
          return 1;
        }
        else
        {
          _window->close();
          return 2;
        }
      }
      default:
        break;
    }
  }
}
