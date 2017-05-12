#include "match.h"
#include "includes/keypresses.hpp"

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
      _scale_factor( 2.5 )
{
  // load the background texture
  if ( !_background->loadFromFile( "assets/StageBackground.png" ) )
    throw "Error loading StageBackground.png";

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

  // the bottom of the character is at 575 pixels.
  _my_y_position =
      575 - ( _player1->get_texture()->getSize().y * _scale_factor );
  _opponent_y_position = _my_y_position;
}

Match::~Match()
{
}

void Match::run()
{
  _window->setFramerateLimit( 7 );
  sf::Sprite background( *_background );
  sf::Sprite player1, player2;
  player1.setScale( player1.getScale().x * 2.5, player1.getScale().y * 2.5 );
  player2.setScale( player2.getScale().x * 2.5, player2.getScale().y * 2.5 );

  // run until either player closes the window or the game ends
  while ( !_player1_events[keys::EXIT] && !_player2_events[keys::EXIT] &&
          !_game_over && _window->isOpen() )
  {

    // update the characters to the new state
    _player1->update( status::WALKING, 'R' );
    _player2->update( status::IDLE, 'L' );
    set_events();

    // set the current textures
    player1.setTexture( *_player1->get_texture() );
    player2.setTexture( *_player2->get_texture() );
    player1.setPosition( _player1_x_position, _my_y_position );
    player2.setPosition( _player2_x_position, _opponent_y_position );

    invoke_events();

    _window->clear();
    // always draw the background first
    _window->draw( background );
    _window->draw( player1 );
    _window->draw( player2 );
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
        add_events( _my_player_num, keys::EXIT );
        my_message.insert( keys::EXIT );
        break;
      }
      case sf::Event::KeyPressed:
      {
        switch ( event.key.code )
        {
          case sf::Keyboard::A:
          {
            add_events( _my_player_num, keys::AKEY );
            my_message.insert( keys::AKEY );
            break;
          }
          case sf::Keyboard::S:
          {
            add_events( _my_player_num, keys::SKEY );
            my_message.insert( keys::SKEY );
            break;
          }
          case sf::Keyboard::D:
          {
            add_events( _my_player_num, keys::DKEY );
            my_message.insert( keys::DKEY );
            break;
          }
          case sf::Keyboard::Space:
          {
            add_events( _my_player_num, keys::SPACEKEY );
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
          add_events( _my_player_num, keys::RMOUSE );
          my_message.insert( keys::RMOUSE );
        }
        else if ( event.mouseButton.button == sf::Mouse::Left )
        {
          add_events( _my_player_num, keys::LMOUSE );
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

  // get opponents events
  their_message = _messenger->get_message();
  //_player2_events = _messenger->get_message();
  for ( auto iter = their_message.begin(); iter != their_message.end(); ++iter )
    add_events( _opponent_player_num, *iter );
}

void Match::add_events( const unsigned int player, const unsigned int key )
{
  if ( player == 1 )
    _player1_events[key] = true;
  else
    _player2_events[key] = true;
}

void Match::invoke_events()
{
  if ( _player1_events[0] )
  {
    _player1_x_position -= 14;
  }
  else if ( _player1_events[2] )
  {
    _player1_x_position += 14;
  }
  if ( _player2_events[0] )
  {
    _player2_x_position -= 14;
  }
  else if ( _player2_events[2] )
  {
    _player2_x_position += 14;
  }
}

void Match::update_players()
{
}
