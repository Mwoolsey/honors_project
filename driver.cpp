#include <iostream>
#include <string>
#include <memory>
#include <set>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "message_handler.h"
#include "main_menu.h"
#include "char_select.h"

#include "includes/keypresses.hpp"

int main( int argc, char* argv[] )
{
  std::string ipaddress = argv[1];  // maybe use boost to validate
  unsigned int port = (unsigned short)std::strtoul( argv[2], NULL, 0 );

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
  /****************************************************************************/

  // create the menu object and pass it the window and texture addresses
  std::shared_ptr<MainMenu> menu( new MainMenu( window, background ) );
  std::shared_ptr<CharSelect> char_select(
      new CharSelect( window, background ) );

  // start the messenger
  std::shared_ptr<MessageHandler> messenger(
      new MessageHandler( ipaddress, port ) );

  int selection = 0;
  bool player2_ready = false;
  std::set<unsigned int> my_message;
  std::set<unsigned int> their_message;
  while ( window->isOpen() )
  {
    // get what button was pressed. If run() returns a 1 then the program
    // is to move on.
    selection = menu->run();
    if ( selection == keys::EXIT )
    {
      window->close();
      goto WINDOW_DONE;
    }
    my_message.insert( keys::STARTED );
    // wait until player 2 is ready
    while ( !player2_ready )
    {
      // send message to player 2 that we are ready
      messenger->send_message( my_message );

      their_message = messenger->get_message();

      // check what the message was and handle accordingly
      if ( their_message.count( keys::STARTED ) )
      {
        std::cout << *their_message.find( keys::STARTED ) << std::endl;
        player2_ready = true;
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
    their_message.clear();

    int character = char_select->run();
    if ( character == keys::EXIT )
    {
      window->close();
    }

    window->close();  // temporary until next phase
  }                   // window loop

WINDOW_DONE:

  // stop threads from handling messages
  messenger->stop_messages();

  window.reset();
  background.reset();
  menu.reset();

  return 0;
}
