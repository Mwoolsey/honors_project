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
      _timer( 99 ),
      _player1_x_position( 250 ),
      _player2_x_position( 650 ),
      _scale_factor( 2.5 ),
      _player1_direction( 'R' ),
      _player2_direction( 'L' ),
      _player1_state( STATE::IDLE ),
      _player2_state( STATE::IDLE )
{
  // load font file
  if ( !_font.loadFromFile( "assets/fonts/yellowjacket.ttf" ) )
    throw "Error loading Font: assets/fonts/yellowjacket.ttf";

  // set font for names
  _player1_name.setFont( _font );
  _player2_name.setFont( _font );
  _timer_text.setFont( _font );
  _timer_text.setPosition( 485, 35 );

  // load the background texture
  if ( !_background->loadFromFile( "assets/StageBackground.png" ) )
    throw "Error loading StageBackground.png";

  // load the match soundtrack
  if ( !_stage_music.openFromFile( "assets/sounds/stage.ogg" ) )
    throw "Error loading soundtrack";

  // create the characters
  try
  {
    if ( my_player_num == 1 )
    {
      _player1_name.setString( my_name );
      _player2_name.setString( opponent_name );
      this->_player1 =
          std::unique_ptr<Character>( new Character( my_name, my_player_num ) );
      this->_player2 = std::unique_ptr<Character>(
          new Character( opponent_name, opponent_num ) );
    }
    else
    {
      _player2_name.setString( my_name );
      _player1_name.setString( opponent_name );
      this->_player1 = std::unique_ptr<Character>(
          new Character( opponent_name, opponent_num ) );
      this->_player2 =
          std::unique_ptr<Character>( new Character( my_name, my_player_num ) );
    }
    _player1_name.setPosition( 155, 5 );
    _player2_name.setPosition(
        850 - ( 25 * ( _player2_name.getString().getSize() - 1 ) ), 5 );
  }
  catch ( char const *e )
  {
    throw e;
  }  // finished creating characters

  // set the players' y_position
  // the bottom of the character is at 575 pixels yet the position functions use
  // the top left corner
  _player1_y_position =
      575 - ( _player1->get_texture()->getSize().y * _scale_factor );
  _player2_y_position =
      575 - ( _player2->get_texture()->getSize().y * _scale_factor );

  // create and position player 1's healthbar
  _player1_healthbar = std::unique_ptr<sf::RectangleShape>(
      new sf::RectangleShape( sf::Vector2f( 300, 25 ) ) );
  _player1_healthbar->setPosition( sf::Vector2f( 150, 40 ) );
  _player1_healthbar->setFillColor( sf::Color::Yellow );
  _player1_health = _player1->get_health();

  // create and position player 2's healthbar
  _player2_healthbar = std::unique_ptr<sf::RectangleShape>(
      new sf::RectangleShape( sf::Vector2f( 300, 25 ) ) );
  _player2_healthbar->setPosition( sf::Vector2f( 550, 40 ) );
  _player2_healthbar->setFillColor( sf::Color::Yellow );
  _player2_health = _player2->get_health();

  // load the jump offsets from the respective files
  set_jump_offsets();

  _window->setFramerateLimit( 5 );
}

Match::~Match()
{
}

int Match::run()
{
  // clear any outstanding messages
  /****************************************************************************/
  std::set<unsigned int> their_message;
  for ( unsigned int i = 0; i < 1000; ++i )
    _messenger->get_message();
  /****************************************************************************/

  _stage_music.play();

  sf::Sprite background( *_background );
  sf::Clock clock;
  int counter;
  // run until either player closes the window or the game ends
  while ( !_player1_events[keys::EXIT] && !_player2_events[keys::EXIT] &&
          !_game_over && _window->isOpen() )
  {
    if ( _timer == 0 )
      return 3;
    // get the time that has passed as seconds, and if a second has passed then
    // decrement the timer and restart the clock
    counter = clock.getElapsedTime().asSeconds();
    if ( counter )
    {
      --_timer;
      clock.restart();
    }
    _timer_text.setString( std::to_string( _timer ) );

    sf::Sprite player1_sprite, player2_sprite;

    update_players( _player1_state, _player2_state );

    set_healthbars();

    if ( _player1_health <= 0 )
    {
      return 2;
    }
    else if ( _player2_health <= 0 )
    {
      return 1;
    }

    // set which events happened since last frame
    set_events();

    // set the sprites current textures, locations, and size
    player1_sprite.setTexture( *_player1->get_texture() );
    player2_sprite.setTexture( *_player2->get_texture() );
    player1_sprite.setPosition( _player1_x_position, _player1_y_position );
    player2_sprite.setPosition( _player2_x_position, _player2_y_position );
    player1_sprite.setScale( _scale_factor, _scale_factor );
    player2_sprite.setScale( _scale_factor, _scale_factor );

    // set the current sprite rectangles to check for collisions later
    _player1_rect = player1_sprite.getGlobalBounds();
    _player2_rect = player2_sprite.getGlobalBounds();

    // update the characters to the new state
    invoke_events( _player1->get_state().first, _player2->get_state().first );

    _window->clear();
    // always draw the background first
    _window->draw( background );
    _window->draw( _player1_name );
    _window->draw( _player2_name );
    _window->draw( *_player1_healthbar );
    _window->draw( *_player2_healthbar );
    _window->draw( _timer_text );
    _window->draw( player1_sprite );
    _window->draw( player2_sprite );
    _window->display();
  }
  return 0;
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
  check_boundaries();
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

  // check which keys were pressed and handle accordingly
  if ( _player1_events[keys::AKEY] )
  {
    _player1_x_position -= 14;
    moving = true;
  }
  if ( _player1_events[keys::DKEY] )
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

  // check if either player is jumping
  if ( _player1_state == STATE::JUMPING )
    invoke_jump( 1 );
  else
    _player1_y_position =
        575 - ( _player1->get_texture()->getSize().y * _scale_factor );
  //  set player directions. This happens once so dont run in player2_events
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

  // check which keys were pressed and handle accordingly
  if ( _player2_events[keys::AKEY] )
  {
    _player2_x_position -= 14;
    moving = true;
  }
  if ( _player2_events[keys::DKEY] )
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

  // check if either player is jumping
  if ( _player2_state == STATE::JUMPING )
    invoke_jump( 2 );
  else
    _player2_y_position =
        575 - ( _player2->get_texture()->getSize().y * _scale_factor );
}

bool Match::collision( void )
{
  // check for collision
  if ( _player1_rect.intersects( _player2_rect ) )
  {
    return true;
  }
  return false;
}

void Match::set_healthbars( void )
{
  // if health has been changed then update health bars
  if ( _player1_health > _player1->get_health() )
  {
    _player1_health = _player1->get_health();
    float p1_scale = _player1_health / 100.0;
    _player1_healthbar->setScale( p1_scale, 1 );
  }
  if ( _player2_health > _player2->get_health() )
  {
    _player2_health = _player2->get_health();
    float p2_scale = _player2_health / 100.0;
    _player2_healthbar->setScale( p2_scale, 1 );
    _player2_healthbar->move( 30, 0 );
  }
}

void Match::check_boundaries( void )
{
  if ( _player1_x_position <= 0 )
    _player1_x_position = 0;
  else if ( _player1_x_position >= ( 1000 - _player1_rect.width ) )
    _player1_x_position = 1000 - _player1_rect.width;
  if ( _player2_x_position <= 0 )
    _player2_x_position = 0;
  else if ( _player2_x_position >= ( 1000 - _player2_rect.width ) )
    _player2_x_position = 1000 - _player2_rect.width;
}

void Match::invoke_jump( int player )
{
  if ( player == 1 )
    _player1_y_position -= _player1_jump_offsets[_player1->get_state().second];
  else
    _player2_y_position -= _player2_jump_offsets[_player2->get_state().second];
}

void Match::set_jump_offsets()
{
  _player1_jump_offsets.clear();
  _player2_jump_offsets.clear();

  // variable for how many jump images there are
  int count;
  // create the file paths to open
  std::string p1_file =
      "assets/" + _player1_name.getString() + "/jump_offsets.txt";
  std::string p2_file =
      "assets/" + _player2_name.getString() + "/jump_offsets.txt";

  std::ifstream ifs;
  ifs.open( p1_file );
  if ( !ifs.is_open() )
    std::cout << "File not opened\n";
  std::string line;
  getline( ifs, line );
  std::stringstream s1( line );
  s1 >> count;
  for ( int i = 0; i < count; ++i )
  {
    int tmp;
    s1 >> tmp;
    _player1_jump_offsets.push_back( tmp );
  }
  ifs.close();

  ifs.open( p2_file );
  getline( ifs, line );
  std::stringstream s2( line );
  s2 >> count;
  for ( int i = 0; i < count; ++i )
  {
    int tmp;
    s2 >> tmp;
    _player2_jump_offsets.push_back( tmp );
  }
  ifs.close();
}
