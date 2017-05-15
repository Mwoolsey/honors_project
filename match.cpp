#include "match.h"
#include "includes/keypresses.hpp"

#include <chrono>
#include <thread>

Match::Match( std::shared_ptr<sf::RenderWindow> window,
              std::shared_ptr<sf::Texture> background,
              std::shared_ptr<MessageHandler> messenger,
              unsigned int my_player_num, unsigned int opponent_num,
              const std::string &my_name, const std::string &opponent_name )
    : _window( window ),
      _background( background ),
      _messenger( messenger ),
      _game_over( false ),
      _my_player_num( my_player_num ),
      _opponent_player_num( opponent_num ),
      _player1_x_position( 250 ),
      _player2_x_position( 650 ),
      _scale_factor( 2.5 ),
      _player1_direction( 'R' ),
      _player2_direction( 'L' ),
      _player1_state( STATE::IDLE ),
      _player2_state( STATE::IDLE )
{
  // load the background texture
  if ( !_background->loadFromFile( "assets/StageBackground.png" ) )
    throw "Error loading StageBackground.png";

  this->_player1_sprite = std::unique_ptr<sf::Sprite>( new sf::Sprite() );
  this->_player2_sprite = std::unique_ptr<sf::Sprite>( new sf::Sprite() );

  try
  {
    this->_player1 =
        std::unique_ptr<Character>( new Character( my_name, my_player_num ) );
    this->_player2 = std::unique_ptr<Character>(
        new Character( opponent_name, opponent_num ) );
  }
  catch ( char const *e )
  {
    throw e;
  }

  // the bottom of the character is at 575 pixels yet the position funcitons use
  // the top left corner
  _player1_y_position =
      575 - ( _player1->get_texture()->getSize().y * _scale_factor );
  _player2_y_position =
      575 - ( _player2->get_texture()->getSize().y * _scale_factor );

  _window->setFramerateLimit( 20 );
  _player1_sprite->setScale( _player1_sprite->getScale().x * 2.5,
                             _player1_sprite->getScale().y * 2.5 );
  _player2_sprite->setScale( _player2_sprite->getScale().x * 2.5,
                             _player2_sprite->getScale().y * 2.5 );
}

Match::~Match()
{
}

void Match::run()
{
  // clear any outstanding messages
  /****************************************************************************/
  std::set<unsigned int> their_message;
  for ( unsigned int i = 0; i < 1000; ++i )
    _messenger->get_message();
  /****************************************************************************/

  sf::Sprite background( *_background );

  int counter = 0;
  // run until either player closes the window or the game ends
  while ( !_player1_events[keys::EXIT] && !_player2_events[keys::EXIT] &&
          !_game_over && _window->isOpen() )
  {
    if ( !( ++counter % 6 ) )
    {
      counter = 0;
      update_players( _player1_state, _player2_state );
      set_events();

      // set the current textures
      _player1_sprite->setTexture( *_player1->get_texture() );
      _player2_sprite->setTexture( *_player2->get_texture() );
      _player1_sprite->setPosition( _player1_x_position, _player1_y_position );
      _player2_sprite->setPosition( _player2_x_position, _player2_y_position );

      // update the characters to the new state
      invoke_events( _player1->get_state(), _player2->get_state() );
    }

    _window->clear();
    // always draw the background first
    _window->draw( background );
    _window->draw( *_player1_sprite );
    _window->draw( *_player2_sprite );
    _window->display();
  }
}

void Match::set_events()
{
  std::set<unsigned int> my_message;
  std::set<unsigned int> their_message;

  // clear the events from the previous iteration
  for ( unsigned int i = 0; i < 1000; ++i )
  {
    _player1_events[i] = false;
    _player2_events[i] = false;
  }

  sf::Event event;
  // get all of my events
  while ( _window->pollEvent( event ) )
  {
    switch ( event.type )
    {
      case sf::Event::Closed:
      {
        my_message.insert( keys::EXIT );
        break;
      }
      case sf::Event::KeyPressed:
      {
        switch ( event.key.code )
        {
          case sf::Keyboard::A:
          {
            my_message.insert( keys::AKEY );
            break;
          }
          case sf::Keyboard::S:
          {
            my_message.insert( keys::SKEY );
            break;
          }
          case sf::Keyboard::D:
          {
            my_message.insert( keys::DKEY );
            break;
          }
          case sf::Keyboard::Space:
          {
            my_message.insert( keys::SPACEKEY );
            break;
          }
          default:
            break;
        }
        break;
      }
      case sf::Event::MouseButtonPressed:
      {
        if ( event.mouseButton.button == sf::Mouse::Right )
        {
          my_message.insert( keys::RMOUSE );
        }
        else if ( event.mouseButton.button == sf::Mouse::Left )
        {
          my_message.insert( keys::LMOUSE );
        }
        break;
      }
      default:
        break;
    }
  }

  // send my events
  _messenger->send_message( my_message );
  for ( auto iter = my_message.begin(); iter != my_message.end(); ++iter )
    add_events( _my_player_num, *iter );

  // get opponents events
  their_message = _messenger->get_message();

  for ( auto iter = their_message.begin(); iter != their_message.end(); ++iter )
    add_events( _opponent_player_num, *iter );

  // print_opponent_events();
}

void Match::add_events( const unsigned int player, const unsigned int key )
{
  if ( player == 1 )
    _player1_events[key] = true;
  else
    _player2_events[key] = true;
}

void Match::invoke_events( const STATE &p1_state, const STATE &p2_state )
{
  // check if either player is in the hit state
  if ( p1_state == STATE::HIT )
    handle_hit( 1 );
  if ( p2_state == STATE::HIT )
    handle_hit( 2 );

  int attacker_finished = check_attacks( p1_state, p2_state );

  if ( attacker_finished == 3 )
  {
    // both players have had state updated
    return;
  }
  else if ( attacker_finished == 2 )
  {
    // update player1 since player2 is already updated
    player1_events( p1_state );
  }
  else if ( attacker_finished == 1 )
  {
    // update player2 since player1 is already updated
    player2_events( p2_state );
  }
  else
  {
    // update both players
    player1_events( p1_state );
    player2_events( p2_state );
  }
}

void Match::update_players( const STATE &p1_state, const STATE &p2_state )
{
  _player1->update( p1_state, _player1_direction );
  _player2->update( p2_state, _player2_direction );
}

void Match::print_opponent_events( void )
{
  for ( unsigned int i = 0; i < 1000; ++i )
  {
    if ( _opponent_player_num == 1 )
    {
      if ( _player1_events[i] )
        std::cout << i << std::endl;
    }
    else
    {
      if ( _player2_events[i] )
        std::cout << i << std::endl;
    }
  }
}

void Match::handle_hit( unsigned int player )
{
  if ( player == 1 )
  {
    if ( _player1_direction == 'R' )
      _player1_x_position -= 14;
    else
      _player1_x_position += 14;
    _player1_state = STATE::HIT;
  }
  else
  {
    if ( _player2_direction == 'R' )
      _player2_x_position -= 14;
    else
      _player2_x_position += 14;
    _player2_state = STATE::HIT;
  }
}

int Match::check_attacks( const STATE &p1_state, const STATE &p2_state )
{
  bool player1_attacking = false;
  bool player2_attacking = false;
  // check if either player is attacking
  // attacks are between JUMPING and CROUCHING
  if ( ( player1_attacking =
             ( STATE::JUMPING < p1_state && STATE::CROUCHING > p1_state ) ) ||
       ( player2_attacking =
             ( STATE::JUMPING < p2_state && STATE::CROUCHING > p2_state ) ) ||
       ( _player1_events[keys::LMOUSE] || _player1_events[keys::RMOUSE] ) ||
       ( _player2_events[keys::LMOUSE] || _player2_events[keys::RMOUSE] ) )
  {
    // dont bother checking attacking states if there is no collision
    if ( collision() )
    {
      if ( player1_attacking &&
           ( p2_state != STATE::HIT && !player2_attacking ) )
      {
        _player2_state = STATE::HIT;
        _player1_state = p1_state;
        return 3;
      }
      else if ( player2_attacking &&
                ( p1_state != STATE::HIT && !player1_attacking ) )
      {
        _player1_state = STATE::HIT;
        _player2_state = p2_state;
        return 3;
      }
    }
    if ( _player1_events[keys::LMOUSE] )
    {
      _player1_state = STATE::PUNCHING;
      return 1;
    }
    else if ( _player2_events[keys::LMOUSE] )
    {
      _player2_state = STATE::PUNCHING;
      return 2;
    }
    else if ( _player1_events[keys::RMOUSE] )
    {
      _player1_state = STATE::KICKING;
      return 1;
    }
    else if ( _player2_events[keys::RMOUSE] )
    {
      _player2_state = STATE::KICKING;
      return 2;
    }
  }
  return 0;
}

void Match::player1_events( const STATE &p1_state )
{
  bool moving = false;
  if ( p1_state == STATE::WALKING || p1_state == STATE::IDLE ||
       p1_state == STATE::JUMPING )
  {
    if ( _player1_events[0] )
    {
      _player1_x_position -= 14;
      moving = true;
    }
    else if ( _player1_events[2] )
    {
      _player1_x_position += 14;
      moving = true;
    }
    if ( _player1_events[keys::SPACEKEY] )
      _player1_state = STATE::JUMPING;
    else if ( p1_state == STATE::WALKING || p1_state == STATE::JUMPING )
      _player1_state = p1_state;
    else if ( moving )
      _player1_state = STATE::WALKING;
    else
      _player1_state = STATE::IDLE;
  }
  // check player directions
  if ( _player1_x_position > _player2_x_position )
  {
    _player1_direction = 'L';
    _player2_direction = 'R';
  }
  else
  {
    _player1_direction = 'R';
    _player2_direction = 'L';
  }
}

void Match::player2_events( const STATE &p2_state )
{
  bool moving = false;
  if ( p2_state == STATE::WALKING || p2_state == STATE::IDLE ||
       p2_state == STATE::JUMPING )
  {
    if ( _player2_events[0] )
    {
      _player2_x_position -= 14;
      moving = true;
    }
    else if ( _player2_events[2] )
    {
      _player2_x_position += 14;
      moving = true;
    }
    if ( _player2_events[keys::SPACEKEY] )
      _player2_state = STATE::JUMPING;
    else if ( p2_state == STATE::WALKING || p2_state == STATE::JUMPING )
      _player2_state = p2_state;
    else if ( moving )
      _player2_state = STATE::WALKING;
    else
      _player2_state = STATE::IDLE;
  }
}

bool Match::collision( void )
{
  // check for collision
  if ( _player1_sprite->getGlobalBounds().intersects(
          _player2_sprite->getGlobalBounds() ) )
  {
    return true;
  }
  return false;
}
