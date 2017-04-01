#include "character.h"
#include <fstream>
#include <sstream>

Character::Character( const std::string &name, unsigned int player )
    : _name( name ),
      _cur_health( 100 ),
      _player( player ),
      _textures( name ),
      _state( IDLE )
{
  // initialize the current execution_position array to a value not used
  std::fill_n( _execution_position, HIT + 1, 99 );
  // set initial position
  _execution_position[IDLE] = 0;

  // create the map that holds how many sub images there are per image
  set_position_counts( name );
}

Character::~Character( void )
{
}

void Character::update(
    std::map<events::CHARACTER_EVENT, bool> &character_events )
{
  if ( character_events[events::CHARACTER_EVENT::NONE] )
    return;
}

unsigned int Character::get_health( void )
{
  return _cur_health;
}

std::shared_ptr<sf::Texture> Character::get_texture( void )
{
  return _character_texture;
}

STATE Character::get_state( void )
{
  return _state;
}

void Character::set_position_counts( const std::string &name )
{
  // read in the position_counts for the character, there are only 7 with 2
  // counts per position
  std::string file = "../assets/" + name + "/position_counts.txt";
  std::ifstream ifs;
  ifs.open( file );
  std::string position;
  unsigned int count;
  for ( int i = 0; i < 14; ++i )
  {
    std::string line;
    getline( ifs, line );
    std::stringstream ss( line );
    ss >> position;
    ss >> count;
    _position_counts.insert( std::make_pair( position, count ) );
  }
  ifs.close();
}
