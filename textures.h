#ifndef TEXTURES_H
#define TEXTURES_H

#include <iostream>

#include <exception>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "images.h"

class Textures
{
 public:
  Textures( const std::string &name );
  sf::Texture get_texture( const std::string &img_name, unsigned int offset );

 private:
  Images _images;
  std::unordered_map<std::string, std::vector<sf::Texture> > _textures;
  // 0: crouch, 1: hit, 2: idle, 3: jump, 4: kick, 5: punch, 6: walk
  std::vector<unsigned int> _image_counts, _widths, _heights;
  std::vector<std::vector<unsigned int> > _offsets, _sub_widths;
  void set_parameters( const std::string &name );
  void set_textures( const std::string &name );
};
#endif /*TEXTURES_H*/
