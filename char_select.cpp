#include "char_select.h"
#include "includes/keypresses.hpp"
#include <stdexcept>

CharSelect::CharSelect( std::shared_ptr<sf::RenderWindow> window,
                        std::shared_ptr<sf::Texture> bg )
    : _window( window ), _background_texture( bg )
{
  // load background image into memory
  if ( !_background_image.loadFromFile( "assets/char_select.png" ) )
    throw std::runtime_error( "Error loading char_select.png into memory" );
  // set border color for rectangles
  sf::Color border( 0, 230, 153 );

  for ( int i = 0; i < 8; ++i )
  {
    // set rectangles up as they will be how we select characters
    _char_highlights[i].setSize( sf::Vector2f( 75, 114 ) );
    _char_highlights[i].setFillColor( sf::Color::Transparent );
    _char_highlights[i].setOutlineThickness( 0 );
    _char_highlights[i].setOutlineColor( border );
  }
  // move rectangles into position
  _char_highlights[RYU].move( 164, 150 );
  _char_highlights[BLANKA].move( 363, 150 );
  _char_highlights[CHUNLI].move( 563, 150 );
  _char_highlights[FEILONG].move( 763, 150 );
  _char_highlights[BALROG].move( 164, 361 );
  _char_highlights[CAMMY].move( 363, 361 );
  _char_highlights[DEEJAY].move( 563, 361 );
  _char_highlights[KEN].move( 763, 361 );
}

CharSelect::~CharSelect()
{
}

int CharSelect::run( void )
{
  // set the background
  if ( !_background_texture->loadFromImage( _background_image ) )
    throw std::runtime_error( "Error loading char_select.png into sprite" );
  _char_select_bg.setTexture( *_background_texture );

  // loop until a character is selected
  int char_selected = NONE;
  while ( char_selected == NONE )
  {
    // get which character is being hovered over
    int over_char = mouse_over_char();

    // check if character is selected
    char_selected = event_handler( over_char );

    _window->clear();
    // have to draw background before anything else
    _window->draw( _char_select_bg );

    // only draw a border if a character is hovered over
    if ( over_char != NONE )
      _window->draw( _char_highlights[over_char] );
    _window->display();
  }
  return char_selected;
}

int CharSelect::mouse_over_char( void )
{
  int character = NONE;
  for ( int i = 0; i < 8; ++i )
  {
    if ( ( _char_highlights[i].getGlobalBounds().contains(
            (sf::Vector2f)sf::Mouse::getPosition( *_window ) ) ) )
    {
      _char_highlights[i].setOutlineThickness( -5 );
      character = i;
    }
    else
    {
      _char_highlights[i].setOutlineThickness( 0 );
    }
  }
  return character;
}

int CharSelect::event_handler( int char_hovered )
{
  // see if any events were caught
  sf::Event event;
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
        return char_hovered;
      }
      default:
      {
        // have to contain default or include every event
      }
    }
  }

  return NONE;
}
