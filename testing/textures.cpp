#include "textures.h"

Textures::Textures( const std::string &name ) : _images( name )
{
  set_textures( name );
}

std::shared_ptr<sf::Texture> Textures::get_texture( const std::string &img_name,
                                                    unsigned int offset )
{
  return _textures[img_name][offset];
}

void Textures::set_parameters( const std::string &name )
{
  unsigned int tmp;
  std::string line;
  std::ifstream file;
  std::vector<std::string> file_names = {"crouch", "hit",   "idle", "jump",
                                         "kick",   "punch", "walk"};
  for ( unsigned int i = 0; i < file_names.size(); ++i )
  {
    file.open( "../assets/" + name + "/" + file_names[i] + ".txt" );
    if ( file.is_open() )
    {
      std::getline( file, line );
      std::stringstream ss( line );

      ss >> tmp;
      _image_counts.push_back( tmp );
      ss >> tmp;
      _widths.push_back( tmp );
      ss >> tmp;
      _heights.push_back( tmp );
      _offsets.push_back( std::vector<unsigned int>() );
      for ( unsigned int j = 0; j < _image_counts[i]; ++j )
      {
        ss >> tmp;
        _offsets[i].push_back( tmp );
      }
      _sub_widths.push_back( std::vector<unsigned int>() );
      for ( unsigned int j = 0; j < _image_counts[i]; ++j )
      {
        ss >> tmp;
        _sub_widths[i].push_back( tmp );
      }
    }
    else
    {
      file.close();
      throw "Error opening " + file_names[i] + ".txt";
    }
    file.close();
  }
}

void Textures::set_textures( const std::string &name )
{
  _textures.insert( std::make_pair(
      "crouch1", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert( std::make_pair(
      "crouch2", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "hit1", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "hit2", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "idle1", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "idle2", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "jump1", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "jump2", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "kick1", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "kick2", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert( std::make_pair(
      "punch1", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert( std::make_pair(
      "punch2", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "walk1", std::vector<std::shared_ptr<sf::Texture> >() ) );
  _textures.insert(
      std::make_pair( "walk2", std::vector<std::shared_ptr<sf::Texture> >() ) );

  // crouch
  for ( unsigned int i = 0; i < _image_counts[0]; ++i )
  {
    // handle the 1's
    _textures["crouch1"].push_back( std::shared_ptr<sf::Texture>() );
    _textures["crouch1"][i]->loadFromImage(
        _images.get_image( "crouch1" ),
        sf::IntRect( _offsets[0][i], 0, _sub_widths[0][i], _heights[0] ) );

    // handle the 2's
  }
  // hit
  for ( unsigned int i = 0; i < _image_counts[1]; ++i )
  {
    // handle the 1's
    _textures["hit1"].push_back( std::shared_ptr<sf::Texture>() );
    _textures["hit1"][i]->loadFromImage(
        _images.get_image( "hit1" ),
        sf::IntRect( _offsets[0][i], 0, _sub_widths[0][i], _heights[0] ) );

    // handle the 2's
  }
  // idle
  for ( unsigned int i = 0; i < _image_counts[2]; ++i )
  {
    // handle the 1's
    _textures["idle1"].push_back( std::shared_ptr<sf::Texture>() );
    _textures["idle1"][i]->loadFromImage(
        _images.get_image( "idle1" ),
        sf::IntRect( _offsets[0][i], 0, _sub_widths[0][i], _heights[0] ) );

    // handle the 2's
  }
  // jump
  for ( unsigned int i = 0; i < _image_counts[3]; ++i )
  {
    // handle the 1's
    _textures["jump1"].push_back( std::shared_ptr<sf::Texture>() );
    _textures["jump1"][i]->loadFromImage(
        _images.get_image( "jump1" ),
        sf::IntRect( _offsets[0][i], 0, _sub_widths[0][i], _heights[0] ) );

    // handle the 2's
  }
  // kick
  for ( unsigned int i = 0; i < _image_counts[4]; ++i )
  {
    // handle the 1's
    _textures["kick1"].push_back( std::shared_ptr<sf::Texture>() );
    _textures["kick1"][i]->loadFromImage(
        _images.get_image( "kick1" ),
        sf::IntRect( _offsets[0][i], 0, _sub_widths[0][i], _heights[0] ) );

    // handle the 2's
  }
  // punch
  for ( unsigned int i = 0; i < _image_counts[5]; ++i )
  {
    // handle the 1's
    _textures["punch1"].push_back( std::shared_ptr<sf::Texture>() );
    _textures["punch1"][i]->loadFromImage(
        _images.get_image( "punch1" ),
        sf::IntRect( _offsets[0][i], 0, _sub_widths[0][i], _heights[0] ) );

    // handle the 2's
  }
  // walk
  for ( unsigned int i = 0; i < _image_counts[6]; ++i )
  {
    // handle the 1's
    _textures["walk1"].push_back( std::shared_ptr<sf::Texture>() );
    _textures["walk1"][i]->loadFromImage(
        _images.get_image( "walk1" ),
        sf::IntRect( _offsets[0][i], 0, _sub_widths[0][i], _heights[0] ) );

    // handle the 2's
  }
}
