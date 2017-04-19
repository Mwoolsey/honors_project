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
      _game_over( false )
{
  if ( my_player_num == 1 )
  {
    this->_my_character =
        std::unique_ptr<Character>( new Character( my_name, my_player_num ) );
    this->_opponent_character = std::unique_ptr<Character>(
        new Character( opponent_name, opponent_num ) );
  }
  else
  {
    try
    {
      this->_opponent_character =
          std::unique_ptr<Character>( new Character( my_name, my_player_num ) );
      this->_my_character = std::unique_ptr<Character>(
          new Character( opponent_name, opponent_num ) );
    }
    catch ( char const *e )
    {
      throw e;
    }
  }
  if ( !_background->loadFromFile( "assets/StageBackground.png" ) )
    throw "Error loading StageBackground.png";
}

Match::~Match()
{
}

void Match::run()
{
  _window->setFramerateLimit( 5 );
  sf::Sprite background( *_background );
  sf::Sprite me, opponent;
  // run until either player closes the window or the game ends
  while ( ( _my_events.find( keys::EXIT ) == _my_events.end() ) &&
          ( _opponent_events.find( keys::EXIT ) == _opponent_events.end() ) &&
          ( !_game_over ) && _window->isOpen() )
  {
    set_events();

    me.setTexture( *_my_character->get_texture() );
    opponent.setTexture( *_opponent_character->get_texture() );

    _window->clear();
    // always draw the background first
    _window->draw( background );
    _window->draw( me );
    _window->draw( opponent );
    _window->display();
    _my_character->update( status::IDLE, 'R' );
    _opponent_character->update( status::IDLE, 'L' );
  }
}

void Match::set_events()
{
  _my_events.clear();
  _opponent_events.clear();

  sf::Event event;
  // get all of my events
  while ( _window->pollEvent( event ) )
  {
    switch ( event.type )
    {
      case sf::Event::Closed:
      {
        _my_events.insert( keys::EXIT );
        break;
      }
      case sf::Event::KeyPressed:
      {
        switch ( event.key.code )
        {
          case sf::Keyboard::A:
          {
            _my_events.insert( keys::AKEY );
            break;
          }
          case sf::Keyboard::S:
          {
            _my_events.insert( keys::SKEY );
            break;
          }
          case sf::Keyboard::D:
          {
            _my_events.insert( keys::DKEY );
            break;
          }
          case sf::Keyboard::Space:
          {
            _my_events.insert( keys::SPACEKEY );
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
          _my_events.insert( keys::RMOUSE );
        else if ( event.mouseButton.button == sf::Mouse::Left )
          _my_events.insert( keys::LMOUSE );
        break;
      }
      default:
        break;
    }
  }

  // send my events
  _messenger->send_message( _my_events );

  // get opponents events
  _opponent_events = _messenger->get_message();
}
