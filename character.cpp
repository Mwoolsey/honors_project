#include "character.h"

Character::Character( const std::string &name, unsigned int player )
    : _name( name ),
      _cur_health( 100 ),
      _max_health( 100 ),
      _images( name ),
      _status( status::IDLE )
{
}

Character::~Character( void )
{
}

void Character::update( const std::vector<int> events )
{
}

unsigned int Character::get_health( void )
{
  return _cur_health;
}

const sf::Vector2f Character::get_position( void )
{
  sf::Vector2f pos( _x_pos, _y_pos );
  return pos;
}

std::shared_ptr<sf::Texture> Character::get_texture( void )
{
  return _character_texture;
}
