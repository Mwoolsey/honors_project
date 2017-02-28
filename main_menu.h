#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

class MainMenu
{
 public:
  MainMenu( std::shared_ptr<sf::RenderWindow> window,
            std::shared_ptr<sf::Texture> bg );
  int run( void );

 private:
  std::shared_ptr<sf::RenderWindow> _window;
  std::shared_ptr<sf::Texture> _background_texture;
  sf::Texture _start_button_up_texture, _start_button_down_texture,
      _join_button_up_texture, _join_button_down_texture;
  sf::Image _background_image;
  sf::Sprite _main_menu_bg, _start_button_sprite, _join_button_sprite;
  // This function handles which texture is loaded for the buttons
  //   it returns which button it is on, returns zero if not on a button
  int mouse_over_box();
  int event_handler( int button_hovered );
};

#endif /*MAIN_MENU_H*/
