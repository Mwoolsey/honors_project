#ifndef MATCH_H
#define MATCH_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <set>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
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

  int run( void );

 private:
  std::shared_ptr<sf::RenderWindow> _window;
  std::shared_ptr<sf::Texture> _background;
  std::shared_ptr<MessageHandler> _messenger;
  std::unique_ptr<Character> _player1;
  std::unique_ptr<Character> _player2;
  std::unique_ptr<sf::RectangleShape> _player1_healthbar;
  std::unique_ptr<sf::RectangleShape> _player2_healthbar;
  bool _player1_events[1000];
  bool _player2_events[1000];
  bool _game_over;
  int _my_player_num, _opponent_player_num;
  unsigned int _timer;

  // these are for positioning the sprites
  float _player1_x_position, _player2_x_position;
  float _player1_y_position, _player2_y_position;

  // this is how much bigger I want the sprites than the original images
  const float _scale_factor;
  // direction the players are facing: 'R' or 'L'
  char _player1_direction, _player2_direction;
  // what the current state of the players are
  STATE _player1_state, _player2_state;
  // player health
  float _player1_health, _player2_health;

  // these hold the increment values for jumping
  std::vector<int> _player1_jump_offsets;
  std::vector<int> _player2_jump_offsets;

  // the current rectangle locations of the players
  sf::Rect<float> _player1_rect, _player2_rect;
  // sfml text to send to the screen
  sf::Text _player1_name;
  sf::Text _player2_name;
  sf::Text _timer_text;
  sf::Font _font;
  sf::Music _stage_music;

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
  void set_healthbars( void );
  void check_boundaries( void );
  void invoke_jump( int player );
  void set_jump_offsets( void );
};
#endif /*MATCH_H*/
