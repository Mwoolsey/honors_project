#include "includes/keypresses.hpp"

#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <set>
#include "message_handler.h"
#include "main_menu.h"
#include "char_select.h"
#include "match.h"

std::string get_character_name( int character_number );

int main( int argc, char* argv[] )
{
  if ( argc != 4 )
  {
    std::cerr << "Wrong number of arguments\n";
    return 0;
  }
  std::string ipaddress = argv[1];  // maybe use boost to validate
  unsigned int port = (unsigned short)std::strtoul( argv[2], NULL, 0 );
  unsigned int my_player_number, opponent_player_number;
  std::string server_or_client = argv[3];
  if ( server_or_client == "S" || server_or_client == "s" )
  {
    my_player_number = 1;
    opponent_player_number = 2;
  }
  else
  {
    my_player_number = 2;
    opponent_player_number = 1;
  }

  //  Initialize window settings
  //////////////////////////////////////////////////////////////////////////////
  int window_width = 1000;
  int window_height = 626;
  std::shared_ptr<sf::RenderWindow> window( new sf::RenderWindow(
      sf::VideoMode( window_width, window_height ), "My Window" ) );
  window->setPosition( sf::Vector2i(
      ( sf::VideoMode::getDesktopMode().width / 2 ) - ( window_width / 2 ),
      ( sf::VideoMode::getDesktopMode().height / 2 ) -
          ( window_height / 2 ) ) );
  window->setVerticalSyncEnabled( true );
  window->setFramerateLimit( 60 );
  /****************************************************************************/

  //  Create textures that need to live for the life of the program
  //////////////////////////////////////////////////////////////////////////////
  std::shared_ptr<sf::Texture> background( new sf::Texture );
  sf::Image waiting;
  waiting.loadFromFile( "assets/waiting.png" );
  /****************************************************************************/

  // create the menu objects and pass them the window and texture addresses
  std::unique_ptr<MainMenu> menu( new MainMenu( window, background ) );
  std::unique_ptr<CharSelect> char_select(
      new CharSelect( window, background ) );

  // start the messenger
  std::shared_ptr<MessageHandler> messenger(
      new MessageHandler( ipaddress, port ) );

  std::string my_character_name;
  std::string opponent_character_name;
  sf::Sprite wait_bg_sprite;
  int character;
  int selection = 0;
  bool opponent_ready = false;
  std::set<unsigned int> my_message;
  std::set<unsigned int> their_message;
  // get what button was pressed. If run() returns a 1 then the program
  // is to move on.
  selection = menu->run();
  if ( selection == keys::EXIT )
  {
    window->close();
    goto WINDOW_DONE;
  }
  my_message.insert( keys::STARTED );

  // show waiting background
  background->loadFromImage( waiting );
  wait_bg_sprite.setTexture( *background );
  window->clear();
  window->draw( wait_bg_sprite );
  window->display();
  // wait until opponent is ready
  while ( !opponent_ready )
  {
    // send message to opponent that we are ready
    messenger->send_message( my_message );

    their_message = messenger->get_message();

    // check what the message was and handle accordingly
    if ( their_message.count( keys::STARTED ) )
    {
      opponent_ready = true;
      break;
    }
    else if ( their_message.count( keys::EXIT ) )
    {
      // if the other player exits, close local session
      // for now
      std::cout << "Player 2 exited\n";
      window->close();
      goto WINDOW_DONE;
    }
  }
  my_message.clear();
  ////////////////////////////////////////////////////////////////////////////

  while ( ( their_message = messenger->get_message() ).size() > 0 )
  {
    // get rid of any outstanding messages
  }

  // start character selection
  ////////////////////////////////////////////////////////////////////////////
  opponent_ready = false;
  character = char_select->run();
  if ( character == keys::EXIT )
  {
    window->close();
  }

  my_character_name = get_character_name( character );

  background->loadFromImage( waiting );
  window->clear();
  window->draw( wait_bg_sprite );
  window->display();
  my_message.insert( character );
  // wait until player 2 is ready
  while ( !opponent_ready )
  {
    // send message to player 2 that we are ready
    messenger->send_message( my_message );

    their_message.clear();
    their_message = messenger->get_message();
    // check what the message was and handle accordingly
    if ( their_message.size() > 0 && their_message.size() < 2 )
    {
      int msg = *their_message.begin();
      // check valid character range
      if ( msg < 8 && msg >= 0 )
      {
        opponent_character_name = get_character_name( msg );
        opponent_ready = true;
        break;
      }
      if ( their_message.count( keys::EXIT ) )
      {
        // if the other player exits, close local session
        // for now
        std::cout << "Player 2 exited\n";
        window->close();
        goto WINDOW_DONE;
      }
    }
  }
  my_message.clear();
  their_message.clear();

  while ( ( their_message = messenger->get_message() ).size() > 0 )
  {
    // get rid of any outstanding messages
  }
  ////////////////////////////////////////////////////////////////////////////

  try
  {
    // start the match
    //
    // here we catch any errors that happened so we can inform the opponent and
    // exit
    Match match( window, background, messenger, my_player_number,
                 opponent_player_number, my_character_name,
                 opponent_character_name );
    match.run();
  }
  catch ( char const* e )
  {
    /* need to send message so opponent knows we had an error */

    std::cerr << e << std::endl;
  }

WINDOW_DONE:
  window->close();

  // stop threads from handling messages
  messenger->stop_messages();

  window.reset();
  background.reset();
  menu.reset();

  return 0;
}

std::string get_character_name( int character_number )
{
  std::string name;
  switch ( character_number )
  {
    case 0:
      return "ryu";
    case 1:
      return "blanka";
    case 2:
      return "chun-li";
    case 3:
      return "fei-long";
    case 4:
      return "balrog";
    case 5:
      return "cammy";
    case 6:
      return "deejay";
    case 7:
      return "ken";
    default:
      break;
  }
  return "ryu";
}
