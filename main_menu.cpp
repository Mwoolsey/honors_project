#include "includes/keypresses.hpp"
#include "main_menu.h"
#include <stdexcept>

MainMenu::MainMenu( std::shared_ptr<sf::RenderWindow> window,
                    std::shared_ptr<sf::Texture> bg )
    : _window( window ), _background_texture( bg )
{
  int window_width = _window->getSize().x;
  int window_height = _window->getSize().y;
  // preload images into memory so texture swap is faster
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

  // set button locations
  _start_button_sprite.setPosition( sf::Vector2f(
      ( ( window_width / 2 ) - ( _start_button_up_texture.getSize().x / 2 ) ),
      window_height / 3 ) );
  _join_button_sprite.setPosition( sf::Vector2f(
      ( ( window_width / 2 ) - ( _join_button_up_texture.getSize().x / 2 ) ),
      window_height / 2 ) );
}

MainMenu::~MainMenu()
{
  _window.reset();
  _background_texture.reset();
}

int MainMenu::run( void )
{
  if ( !_background_texture->loadFromImage( _background_image ) )
    throw std::runtime_error( "Error loading main_menu_bg.png" );

  // set the background sprite to the global background texture which now
  // holds the main menu background
  _main_menu_bg.setTexture( *_background_texture );

  int button_clicked = 0;
  while ( button_clicked == 0 )
  {
    int button_selection = mouse_over_box();

    // handle any events and pass which button, if any, the mouse is over

    _window->clear();

    // Draw everything to the window
    _window->draw( _main_menu_bg );
    _window->draw( _start_button_sprite );
    _window->draw( _join_button_sprite );
    _window->display();
    button_clicked = event_handler( button_selection );
  }
  return button_clicked;
}

// returns which box, if any, the mouse is hovering over
// 0 = no button
// 1 = start button
// 2 = join button
int MainMenu::mouse_over_box()
{
  if ( ( _start_button_sprite.getGlobalBounds().contains(
          (sf::Vector2f)sf::Mouse::getPosition( *_window ) ) ) )
  {
    _start_button_sprite.setTexture( _start_button_down_texture );
    return 1;
  }
  if ( ( _join_button_sprite.getGlobalBounds().contains(
          (sf::Vector2f)sf::Mouse::getPosition( ( *_window ) ) ) ) )
  {
    _join_button_sprite.setTexture( _join_button_down_texture );
    return 2;
  }
  _join_button_sprite.setTexture( _join_button_up_texture );
  _start_button_sprite.setTexture( _start_button_up_texture );
  return 0;
}

int MainMenu::event_handler( int button_hovered )
{
  sf::Event event;

  // stay on menu until a valid button was pressed
  while ( _window->pollEvent( event ) )
  {
    switch ( event.type )
    {
      case sf::Event::Closed:
      {
        _window->close();
        return keys::EXIT;
      }
      case sf::Event::MouseButtonReleased:
      {
        if ( button_hovered == 1 )
        {
          return 1;
          break;
        }
        else if ( button_hovered == 2 )
        {
          return keys::EXIT;
        }
        else
        {
          return 0;
        }
      }
      default:
        break;
    }
  }
  return 0;
}
