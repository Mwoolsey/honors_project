#include "character.h"
#include <fstream>
#include <sstream>

Character::Character( const std::string &name, unsigned int player ) try
    : _name( name ),
      _cur_health( 100 ),
      _textures( name ),
      _state( IDLE )
{
  // initialize the current execution_position array to a value not used
  std::fill_n( _execution_position, HIT + 1, 99 );
  // set initial position
  _execution_position[IDLE] = 0;

  // this will be the starting image
  std::string img_name = ( ( player == 1 ) ? "idle1" : "idle2" );

  // set the initial character texture to idle
  _character_texture =
      std::make_shared<sf::Texture>( _textures.get_texture( img_name, 0 ) );

  // create the map that holds how many sub images there are per image
  set_position_counts( name );
}
catch ( char const *e )
{
  throw e;
}

Character::~Character( void )
{
}

void Character::update( const STATE &state, char facing )
{
  // get the string for the image to match the state
  std::string img_name;
  switch ( state )
  {
    case IDLE:
    {
      img_name = "idle";
      break;
    }
    case WALKING:
    {
      img_name = "walk";
      break;
    }
    case JUMPING:
    {
      img_name = "jump";
      break;
    }
    case PUNCHING:
    {
      img_name = "punch";
      break;
    }
    case KICKING:
    {
      img_name = "kick";
      break;
    }
    case CROUCHING:
    {
      img_name = "crouch";
      break;
    }
    case HIT:
    {
      img_name = "hit";
      break;
    }
    default:
      break;
  }

  // if we are facing right, append a 1 to the img_name, else append a 2
  if ( facing == 'R' )
    img_name.append( "1" );
  else
    img_name.append( "2" );

  // if we are still in the same state
  if ( state == _state )
  {
    // if the execution_position is less than max, increment it. Else reset it
    // to IDLE
    if ( _execution_position[state] < _position_counts[img_name] - 1 )
    {
      ++_execution_position[state];
    }
    else  // we have finished the loop of whatever image we are on
    {
      // set the current state to not being used
      //_execution_position[state] = 99;
      // reset the IDLE state to the beggining of the loop and change to IDLE
      _execution_position[state] = 0;
      if ( state == HIT )
        _state = IDLE;
    }
  }
  else  // the new state doesn't match the current state
  {
    _execution_position[state] = 0;
    // decrement health if character was hit
    if ( state == HIT )
      _cur_health -= 10;

    // start the image loop counter for the state we are in
    _state = state;
  }

  // set the new current texture
  _character_texture = std::make_shared<sf::Texture>(
      _textures.get_texture( img_name, _execution_position[state] ) );
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
  // counts per position (right and left)
  std::string file = "assets/" + name + "/position_counts.txt";
  std::ifstream ifs;
  ifs.open( file );
  std::string position;
  unsigned int count;
  // might change to while getline in case more images are added later
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
