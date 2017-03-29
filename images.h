#ifndef IMAGES_H
#define IMAGES_H

#include <exception>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

/*
 * This class is intended to be used to bring all the images for a certain
 * character into memory and make them easily accessible
 */

class Images
{
 public:
  Images( const std::string &name );
  sf::Image get_image( const std::string &img_name );

 private:
  std::unordered_map<std::string, sf::Image> _images;
};
#endif /*IMAGES_H*/
