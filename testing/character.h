#ifndef CHARACTER_H
#define CHARACTER_H

#include "../includes/status.hpp"
#include "../includes/events.hpp"

#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "textures.h"

using namespace events;
using namespace status;

class Character
{
 public:
  Character( const std::string &name, unsigned int player );
  ~Character( void );
  void update( const std::vector<bool> events );
  unsigned int get_health( void );
  const sf::Vector2f get_position( void );
  std::shared_ptr<sf::Texture> get_texture( void );

 private:
  std::string _name;
  unsigned int _cur_health;
  int _x_pos, _y_pos;
  std::shared_ptr<sf::Texture> _character_texture;
  Textures _textures;
  // HIT is the largest status so we will be able to just index the array based
  // on status to find out what index of the status we are in the plus one is
  // because we need HIT to be accessable and not out of bounds since the array
  // is zero based
  unsigned int _execution_position[HIT + 1];
  std::unordered_map<std::string, unsigned int> _position_counts;
  STATUS _status;

  void set_position_counts( const std::string &name );
};
#endif /*CHARACTER_H*/
