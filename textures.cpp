#include "textures.h"

Textures::Textures( const std::string &name ) try : _images( name )
{
  try
  {
    set_parameters( name );
    set_textures( name );
  }
  catch ( char const *e )
  {
    throw e;
  }
}
catch ( char const *e )
{
  throw e;
}

sf::Texture Textures::get_texture( const std::string &img_name,
                                   unsigned int offset )
{
  return _textures[img_name][offset];
}

void Textures::set_parameters( const std::string &name )
{
  unsigned int tmp;
  std::string line;
  std::ifstream file;
  std::vector<std::string> file_names_forward = {
      "crouch1", "hit1", "idle1", "jump1", "kick1", "punch1", "walk1"};
  std::vector<std::string> file_names_reverse = {
      "crouch2", "hit2", "idle2", "jump2", "kick2", "punch2", "walk2"};

  // handle forward facing textures
  for ( unsigned int i = 0; i < file_names_forward.size(); ++i )
  {
    file.open( "assets/" + name + "/" + file_names_forward[i] + ".txt" );  //
    if ( file.is_open() )
    {
      std::getline( file, line );
      std::stringstream ss( line );

      ss >> tmp;
      _image_counts.push_back( tmp );  //
      ss >> tmp;
      _widths.push_back( tmp );  //
      ss >> tmp;
      _heights.push_back( tmp );  //
      _forward_offsets.push_back( std::vector<unsigned int>() );
      for ( unsigned int j = 0; j < _image_counts[i]; ++j )
      {
        ss >> tmp;
        _forward_offsets[i].push_back( tmp );
      }
      _forward_sub_widths.push_back( std::vector<unsigned int>() );
      for ( unsigned int j = 0; j < _image_counts[i]; ++j )
      {
        ss >> tmp;
        _forward_sub_widths[i].push_back( tmp );
      }
    }
    else
    {
      file.close();
      throw "Error opening " + file_names_forward[i] + ".txt";
    }
    file.close();
  }

  // handle reverse facing textures
  for ( unsigned int i = 0; i < file_names_reverse.size(); ++i )
  {
    file.open( "assets/" + name + "/" + file_names_reverse[i] + ".txt" );  //
    if ( file.is_open() )
    {
      std::getline( file, line );
      std::stringstream ss( line );

      // don't need to change image counts, widths isn't used right now, and
      // heights are the same
      ss >> tmp;
      //_image_counts.push_back( tmp );
      ss >> tmp;
      //_widths.push_back( tmp );//
      ss >> tmp;
      //_heights.push_back( tmp );//
      _reverse_offsets.push_back( std::vector<unsigned int>() );
      for ( unsigned int j = 0; j < _image_counts[i]; ++j )
      {
        ss >> tmp;
        _reverse_offsets[i].push_back( tmp );
      }
      _reverse_sub_widths.push_back( std::vector<unsigned int>() );
      for ( unsigned int j = 0; j < _image_counts[i]; ++j )
      {
        ss >> tmp;
        _reverse_sub_widths[i].push_back( tmp );
      }
    }
    else
    {
      file.close();
      throw "Error opening " + file_names_reverse[i] + ".txt";
    }
    file.close();
  }
}

void Textures::set_textures( const std::string &name )
{
  _textures.insert( std::make_pair( "crouch1", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "crouch2", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "hit1", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "hit2", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "idle1", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "idle2", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "jump1", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "jump2", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "kick1", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "kick2", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "punch1", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "punch2", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "walk1", std::vector<sf::Texture>() ) );
  _textures.insert( std::make_pair( "walk2", std::vector<sf::Texture>() ) );

  // crouch
  for ( unsigned int i = 0; i < _image_counts[0]; ++i )
  {
    // handle the 1's
    _textures["crouch1"].push_back( sf::Texture() );
    if ( !_textures["crouch1"].back().loadFromImage(
              _images.get_image( "crouch1" ),
              sf::IntRect( _forward_offsets[0][i], 0, _forward_sub_widths[0][i],
                           _heights[0] ) ) )
    {
      throw "Error loading crouch1 texture: " + i;
    }

    // handle the 2's
    _textures["crouch2"].push_back( sf::Texture() );
    if ( !_textures["crouch2"].back().loadFromImage(
              _images.get_image( "crouch2" ),
              sf::IntRect( _reverse_offsets[0][i], 0, _reverse_sub_widths[0][i],
                           _heights[0] ) ) )
    {
      throw "Error loading crouch2 texture: " + i;
    }
  }
  std::reverse( _textures["crouch2"].begin(), _textures["crouch2"].end() );
  // hit
  for ( unsigned int i = 0; i < _image_counts[1]; ++i )
  {
    // handle the 1's
    _textures["hit1"].push_back( sf::Texture() );
    if ( !_textures["hit1"].back().loadFromImage(
              _images.get_image( "hit1" ),
              sf::IntRect( _forward_offsets[1][i], 0, _forward_sub_widths[1][i],
                           _heights[1] ) ) )
    {
      throw "Error loading hit1 texture: " + i;
    }

    // handle the 2's
    _textures["hit2"].push_back( sf::Texture() );
    if ( !_textures["hit2"][i].loadFromImage(
              _images.get_image( "hit2" ),
              sf::IntRect( _reverse_offsets[1][i], 0, _reverse_sub_widths[1][i],
                           _heights[1] ) ) )
    {
      throw "Error loading hit2 texture: " + i;
    }
  }
  std::reverse( _textures["hit2"].begin(), _textures["hit2"].end() );
  // idle
  for ( unsigned int i = 0; i < _image_counts[2]; ++i )
  {
    // handle the 1's
    _textures["idle1"].push_back( sf::Texture() );
    if ( !_textures["idle1"][i].loadFromImage(
              _images.get_image( "idle1" ),
              sf::IntRect( _forward_offsets[2][i], 0, _forward_sub_widths[2][i],
                           _heights[2] ) ) )
    {
      throw "Error loading idle1 texture: " + i;
    }

    // handle the 2's
    _textures["idle2"].push_back( sf::Texture() );
    if ( !_textures["idle2"][i].loadFromImage(
              _images.get_image( "idle2" ),
              sf::IntRect( _reverse_offsets[2][i], 0, _reverse_sub_widths[2][i],
                           _heights[2] ) ) )
    {
      throw "Error loading idle2 texture: " + i;
    }
  }
  std::reverse( _textures["idle2"].begin(), _textures["idle2"].end() );
  // jump
  for ( unsigned int i = 0; i < _image_counts[3]; ++i )
  {
    // handle the 1's
    _textures["jump1"].push_back( sf::Texture() );
    if ( !_textures["jump1"][i].loadFromImage(
              _images.get_image( "jump1" ),
              sf::IntRect( _forward_offsets[3][i], 0, _forward_sub_widths[3][i],
                           _heights[3] ) ) )
    {
      throw "Error loading jump1 texture: " + i;
    }

    // handle the 2's
    _textures["jump2"].push_back( sf::Texture() );
    if ( !_textures["jump2"][i].loadFromImage(
              _images.get_image( "jump2" ),
              sf::IntRect( _reverse_offsets[3][i], 0, _reverse_sub_widths[3][i],
                           _heights[3] ) ) )
    {
      throw "Error loading jump2 texture: " + i;
    }
  }
  std::reverse( _textures["jump2"].begin(), _textures["jump2"].end() );
  // kick
  for ( unsigned int i = 0; i < _image_counts[4]; ++i )
  {
    // handle the 1's
    _textures["kick1"].push_back( sf::Texture() );
    if ( !_textures["kick1"][i].loadFromImage(
              _images.get_image( "kick1" ),
              sf::IntRect( _forward_offsets[4][i], 0, _forward_sub_widths[4][i],
                           _heights[4] ) ) )
    {
      throw "Error loading kick1 texture: " + i;
    }

    // handle the 2's
    _textures["kick2"].push_back( sf::Texture() );
    if ( !_textures["kick2"][i].loadFromImage(
              _images.get_image( "kick2" ),
              sf::IntRect( _reverse_offsets[4][i], 0, _reverse_sub_widths[4][i],
                           _heights[4] ) ) )
    {
      throw "Error loading kick2 texture: " + i;
    }
  }
  std::reverse( _textures["kick2"].begin(), _textures["kick2"].end() );
  // punch
  for ( unsigned int i = 0; i < _image_counts[5]; ++i )
  {
    // handle the 1's
    _textures["punch1"].push_back( sf::Texture() );
    if ( !_textures["punch1"][i].loadFromImage(
              _images.get_image( "punch1" ),
              sf::IntRect( _forward_offsets[5][i], 0, _forward_sub_widths[5][i],
                           _heights[5] ) ) )
    {
      throw "Error loading punch1 texture: " + i;
    }

    // handle the 2's
    _textures["punch2"].push_back( sf::Texture() );
    if ( !_textures["punch2"][i].loadFromImage(
              _images.get_image( "punch2" ),
              sf::IntRect( _reverse_offsets[5][i], 0, _reverse_sub_widths[5][i],
                           _heights[5] ) ) )
    {
      throw "Error loading punch2 texture: " + i;
    }
  }
  std::reverse( _textures["punch2"].begin(), _textures["punch2"].end() );

  // walk
  for ( unsigned int i = 0; i < _image_counts[6]; ++i )
  {
    // handle the 1's
    _textures["walk1"].push_back( sf::Texture() );
    if ( !_textures["walk1"][i].loadFromImage(
              _images.get_image( "walk1" ),
              sf::IntRect( _forward_offsets[6][i], 0, _forward_sub_widths[6][i],
                           _heights[6] ) ) )
    {
      throw "Error loading walk1 texture: " + i;
    }

    // handle the 2's
    _textures["walk2"].push_back( sf::Texture() );
    if ( !_textures["walk2"][i].loadFromImage(
              _images.get_image( "walk2" ),
              sf::IntRect( _reverse_offsets[6][i], 0, _reverse_sub_widths[6][i],
                           _heights[6] ) ) )
    {
      throw "Error loading walk2 texture: " + i;
    }
  }
  std::reverse( _textures["walk2"].begin(), _textures["walk2"].end() );
}
