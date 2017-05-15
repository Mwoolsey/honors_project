#ifndef MATCH_H
#define MATCH_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "message_handler.h"

#include "character.h"

class Match
{
 public:
  Match( std::shared_ptr<sf::RenderWindow> window,
         std::shared_ptr<sf::Texture> background,
         std::shared_ptr<MessageHandler> messenger, unsigned int my_player_num,
         unsigned int opponent_num, const std::string &my_name,
         const std::string &opponent_name );
  ~Match( void );

  void run( void );

 private:
  std::shared_ptr<sf::RenderWindow> _window;
  std::shared_ptr<sf::Texture> _background;
  std::shared_ptr<MessageHandler> _messenger;
  std::unique_ptr<Character> _player1;
  std::unique_ptr<Character> _player2;
  std::unique_ptr<sf::Sprite> _player1_sprite;
  std::unique_ptr<sf::Sprite> _player2_sprite;
  bool _player1_events[1000];
  bool _player2_events[1000];
  bool _game_over;
  int _my_player_num, _opponent_player_num;
  float _player1_x_position, _player2_x_position;
  float _player1_y_position, _player2_y_position;
  const float _scale_factor;
  char _player1_direction, _player2_direction;
  STATE _player1_state, _player2_state;

  void set_events( void );
  void add_events( const unsigned int player, const unsigned int key );
  void invoke_events( const STATE &p1_state, const STATE &p2_state );
  void update_players( const STATE &p1_state, const STATE &p2_state );
  void print_opponent_events( void );
  void handle_hit( unsigned int player );
  // returns 0 for neither players finished, 1 for player1 is finished, 2 for
  // player2 finished, 3 for both finished
  int check_attacks( const STATE &p1_state, const STATE &p2_state );
  void player1_events( const STATE &p1_state );
  void player2_events( const STATE &p2_state );
  bool collision( void );
};
#endif /*MATCH_H*/
