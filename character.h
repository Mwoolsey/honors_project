#ifndef CHARACTER_H
#define CHARACTER_H

#include "includes/status.hpp"

#include <string>
#include <memory>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unordered_map>

#include "images.h"

class Character
{
 public:
  Character( const std::string &name, unsigned int player );
  ~Character( void );
  void update( const std::vector<int> events );
  unsigned int get_health( void );
  const sf::Vector2f get_position( void );
  std::shared_ptr<sf::Texture> get_texture( void );

 private:
  std::string _name;
  unsigned int _cur_health, _max_health;
  int _x_pos, _y_pos;
  std::shared_ptr<sf::Texture> _character_texture;
  Images _images;
  status::STATUS _status;
};
#endif /*CHARACTER_H*/
