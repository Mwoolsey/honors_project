#include "includes/keypresses.hpp"
#include "message_handler.h"
#include <stdexcept>

MessageHandler::MessageHandler( const std::string ipaddress,
                                const unsigned short port )
    : _ipaddress( ipaddress ),
      _port( port ),
      _thread( new sf::Thread( &MessageHandler::handle_messages, this ) )
{
  _packet_send.clear();
  _packet_receive.clear();
  _socket.setBlocking( false );
  init();
  _thread->launch();
}

MessageHandler::~MessageHandler( void )
{
  if ( _thread )
  {
    _thread->wait();
    delete _thread;
  }
}

void MessageHandler::handle_messages( void )
{
  while ( !_quit )
  {
    // lock the packet to check size and deal with message
    _send_mutex.lock();
    if ( _packet_send.getDataSize() > 0 )
    {
      _socket.send( _packet_send, _ipaddress, _port );
      _packet_send.clear();
    }
    _send_mutex.unlock();

    // handle recieving messages
    sf::IpAddress rcvd_addr;
    unsigned short rcvd_port;

    _receive_mutex.lock();
    _socket.receive( _packet_receive, rcvd_addr, rcvd_port );
    if ( _packet_receive.getDataSize() > 0 )
    {
      unsigned int tmp;
      while ( _packet_receive >> tmp )
      {
        _unique_message.insert( tmp );
      }
    }
    _receive_mutex.unlock();
  }
}

void MessageHandler::init( void )
{
  if ( _socket.bind( _port ) != sf::Socket::Done )
  {
    throw "Error Binding Socket";
  }
}

const std::set<unsigned int> MessageHandler::get_message( void )
{
  // get the contents of the unique_message and return them and clear
  // unique_message
  _receive_mutex.lock();
  std::set<unsigned int> message( _unique_message );
  _unique_message.clear();
  _receive_mutex.unlock();
  return message;
}

void MessageHandler::send_message( const std::set<unsigned int> message )
{
  // make sure something is being sent
  if ( !message.empty() )
  {
    // add message to packet to be sent
    _send_mutex.lock();
    for ( auto it = message.begin(); it != message.end(); ++it )
    {
      _packet_send << *it;
    }
    _send_mutex.unlock();
  }
}

void MessageHandler::stop_messages()
{
  // tell the other player we have quit
  std::set<unsigned int> quit;
  quit.insert( 999 );
  send_message( quit );
  _quit = true;
}
