#include "images.h"
#include <iostream>

Images::Images( const std::string &name )
{
  // create the map
  _images.insert(
      std::make_pair<std::string, sf::Image>( "idle1", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "idle2", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "crouch1", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "crouch2", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "jump1", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "jump2", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "kick1", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "kick2", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "punch1", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "punch2", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "walk1", sf::Image() ) );
  _images.insert(
      std::make_pair<std::string, sf::Image>( "walk2", sf::Image() ) );

  // load all the images into the map
  for ( auto it = _images.begin(); it != _images.end(); ++it )
  {
    if ( it->second.loadFromFile( "assets/" + name + "/" + it->first +
                                  ".png" ) )
      std::cerr << "Error loading image assets/" + name + "/idle1.png\n";
  }
}

sf::Image Images::get_image( const std::string &img_name )
{
  return _images[img_name];
}
