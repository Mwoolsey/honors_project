/*
 * A character holds the following information:
 *  Current health, Current texture, and Current state
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include "includes/status.hpp"
#include "includes/events.hpp"

#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "textures.h"

using namespace status;

class Character
{
 public:
  /*
   * This is the constructor for each character. It will initialize all needed
   * variables.
   * @param const std::string &name - the name of the character being used
   * @param unsigned int player - Player 1 or Player 2, used to initialize
   *                              textures
   */
  Character( const std::string &name, unsigned int player );
  ~Character( void );

  /*
   * This function will update all the current information for the character
   * including: current health, current texture, and current state
   * @param const STATE &state - The new state to be updated to
   * @param char facing - Which way the character is facing, R or L
   */
  void update( const STATE &state, char facing );

  /*
   * This function get the current value of the characters health
   * @return unsigned int - The characters current health
   */
  unsigned int get_health( void );

  /*
   * This function is used to get the current texture that the character is
   * using
   * @return shared_ptr<Texture> - The characters current texture
   */
  std::shared_ptr<sf::Texture> get_texture( void );

  /*
   * This function is used to get the current state that the character is in
   * @return STATE - The current state of the character
   */
  STATE get_state( void );

 private:
  std::string _name;
  unsigned int _cur_health;
  std::shared_ptr<sf::Texture> _character_texture;
  // right now there are no images above 32
  std::string _image_names[40];

  // an object to hold all the textures for the given character
  Textures _textures;
  STATE _state;

  /* HIT is the largest status so we will be able to just index the array based
   * on status to find out what index of the status we are in the plus one is
   * because we need HIT to be accessable and not out of bounds since the array
   * is zero based
   */
  unsigned int _execution_position[HIT + 1];

  // the sub-image counts for each image/position
  std::unordered_map<std::string, unsigned int> _position_counts;

  void set_position_counts( const std::string &name );
};
#endif /*CHARACTER_H*/
