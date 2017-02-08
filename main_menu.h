#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Main_menu
{
 public:
  Main_menu( sf::RenderWindow* window, sf::Texture* bg );
  void run( void );

 private:
  sf::RenderWindow* _window;
  sf::Texture* _background_texture;
  sf::Texture _options_button_texture, _start_button_texture;
  sf::Image _background_image;
  sf::Sprite _main_menu_bg;
};

#endif /*MAIN_MENU_H*/
