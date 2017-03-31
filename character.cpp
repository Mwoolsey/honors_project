#include "character.h"
#include <fstream>
#include <sstream>

Character::Character( const std::string &name, unsigned int player )
    : _name( name ), _cur_health( 100 ), _images( name ), _status( IDLE )
{
  std::cout << "In constructor\n";
  // initialize the current execution_position array to a value not used
  std::fill_n( execution_position, HIT, 99 );

  // read in the position_counts for the character, there are only 7 positions
  std::ifstream ifs( "assets/" + name + "position_counts.txt" );
  std::string position;
  unsigned int count;
  for ( int i = 0; i < 7; ++i )
  {
    std::string line;
    getline( ifs, line );
    std::stringstream ss( line );
    ss >> position;
    ss >> count;
    std::cout << position + " " << count << std::endl;
  }
}

Character::~Character( void )
{
}

void Character::update( const std::vector<CHARACTER_EVENT> events )
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
