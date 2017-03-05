#ifndef CHAR_SELECT_H
#define CHAR_SELECT_H

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

class CharSelect
{
 public:
  CharSelect( std::shared_ptr<sf::RenderWindow> window,
              std::shared_ptr<sf::Texture> bg );
  ~CharSelect();
  int run( void );

 private:
  std::shared_ptr<sf::RenderWindow> _window;
  std::shared_ptr<sf::Texture> _background_texture;
  sf::Image _background_image;
  sf::Sprite _char_select_bg;
  sf::RectangleShape _char_highlights[8];
  int mouse_over_char( void );
  int event_handler( int char_hovered );
  enum
  {
    RYU = 0,
    BLANKA = 1,
    CHUNLI = 2,
    FEILONG = 3,
    BALROG = 4,
    CAMMY = 5,
    DEEJAY = 6,
    KEN = 7,
    NONE = 8
  };
};

#endif /*CHAR_SELECT_H*/
