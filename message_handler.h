#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <iostream>
#include <string>
#include <set>
#include <memory>
#include <SFML/Network.hpp>

class MessageHandler
{
 public:
  /*
   * This is the constructor, no default is used
   * @param const string ipaddress - the ipaddress to connect the socket to.
   * @param const unsigned short - the port number to connect the socket to.
   */
  MessageHandler( const std::string ipaddress, const unsigned short port );

  // Destructor to handle any threads or pointers
  ~MessageHandler( void );

  /*
   * Function get_message is used to retrieve the current events that the
   * other player has sent. It uses a set to keep the events unique between
   * frames.
   *
   * @return const set get_message - A unique list of events from player 2
   */
  const std::set<unsigned int> get_message( void );

  /*
   * Function send_message is used to send the events from our world to
   * player 2. Again we use a set to include only unique keypresses.
   *
   * @param const set message - The message to send to player 2
   */
  void send_message( const std::set<unsigned int> message );

  /*
   * Funcion stop_messages is used to signal that the program is done with
   * messaging. The thread is looping to handle messages until this function
   * is called, which just adjusts the flag accordingly.
   */
  void stop_messages( void );

  /*
   * Function set_blocking changes the blocking state of the socket based
   * on the parameter.
   *
   * @param bool set - the state of the socket blocking
   */
  void set_blocking( bool set )
  {
    _socket.setBlocking( set );
  };

 private:
  std::string _ipaddress;
  unsigned short _port;
  sf::Packet _packet_send;
  sf::Packet _packet_receive;
  sf::UdpSocket _socket;
  sf::Mutex _send_mutex;
  sf::Mutex _receive_mutex;
  sf::Thread* _thread;  // sfml threads need to be raw pointers
  bool _quit;
  std::set<unsigned int> _unique_message;
  void handle_messages( void );
  void init( void );
};
#endif /*MESSAGE_HANDLER_H*/
