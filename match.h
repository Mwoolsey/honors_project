#ifndef MATCH_H
#define MATCH_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <set>

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
  std::unique_ptr<Character> _my_character;
  std::unique_ptr<Character> _opponent_character;
  std::shared_ptr<sf::Texture> _my_texture;
  std::shared_ptr<sf::Texture> _opponent_texture;
  std::set<unsigned int> _my_events;
  std::set<unsigned int> _opponent_events;
  bool _game_over;

  void set_events();
};
#endif /*MATCH_H*/
