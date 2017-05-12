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
  ~Match();

  void run();

 private:
  std::shared_ptr<sf::RenderWindow> _window;
  std::shared_ptr<sf::Texture> _background;
  std::shared_ptr<MessageHandler> _messenger;
  std::unique_ptr<Character> _player1;
  std::unique_ptr<Character> _player2;
  bool _player1_events[1000];
  bool _player2_events[1000];
  bool _game_over;
  int _my_player_num, _opponent_player_num;
  float _player1_x_position, _player2_x_position;
  float _my_y_position, _opponent_y_position;
  const float _scale_factor;

  void set_events();
  void add_events( const unsigned int player, const unsigned int key );
  void invoke_events();
  void update_players();
};
#endif /*MATCH_H*/
