/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>

#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;
using namespace Zeni;

/*** INI File Parsers ***/

/// Video_Options stores information used to control the creation of the Video system and Textures
struct Video_Options {
  Video_Base::VIDEO_MODE vm;
  int anisotropy, multisampling;
  bool vsync, bilinear, mipmapping, fullscreen;
  int width, height;
};

struct Corrupt_INI : public Error {
  Corrupt_INI() : Error("Corrupt INI file could not be parsed!") {}
};

/// A basic INI parser for Video_Options
static Video_Options handle_video_options(istream &is) {
  Video_Options vopts = {Video::get_video_mode(),
    Textures::get_anisotropic_filtering(),
    Video::get_multisampling(),
    Video::get_vertical_sync(),
    Textures::get_bilinear_filtering(),
    Textures::get_mipmapping(),
    Video::is_fullscreen(),
    Video::get_screen_width(),
    Video::get_screen_height()};

  string line;
  for(getline(is, line); is && !line.empty(); getline(is, line)) {
    {
      string l2;
      for(unsigned int i = 0; i < line.length(); ++i)
        if(line[i] != '\r')
          l2 += line[i];
      line = l2;
    }

    if(line.empty())
      break;
    if(line[0] == '#')
      continue;

    int break_point = int(line.find('='));
    if(break_point == -1) {
      cerr << "No equals sign: " << line << endl;
      throw Corrupt_INI();
    }

    string id = line.substr(0, break_point),
      value = line.substr(break_point + 1, line.length() - break_point - 1);

    if(id == "RenderingEngine") {
#ifndef DISABLE_GL
      if(value == "OpenGL")
        vopts.vm = Video_Base::ZENI_VIDEO_GL;
      else
#endif
#ifndef DISABLE_DX9
        if(value == "DX9")
          vopts.vm = Video_Base::ZENI_VIDEO_DX9;
        else
#endif
          vopts.vm = Video_Base::ZENI_VIDEO_ANY;
    }
    else if(id == "Anisotropy")
      vopts.anisotropy = atoi(value.c_str());
    else if(id == "Multisampling")
      vopts.multisampling = atoi(value.c_str());
    else if(id == "V-Sync")
      vopts.vsync = atoi(value.c_str()) > 0;
    else if(id == "Bilinear")
      vopts.bilinear = atoi(value.c_str()) > 0;
    else if(id == "Mipmapping")
      vopts.mipmapping = atoi(value.c_str()) > 0;
    else if(id == "FullScreen")
      vopts.fullscreen = atoi(value.c_str()) > 0;
    else if(id == "Width")
      vopts.width = atoi(value.c_str());
    else if(id == "Height")
      vopts.height = atoi(value.c_str());
    else
      cerr << "Unrecognized Video Option: " << line << endl;
  }

  return vopts;
}

static void load_ini() {
  Video_Options vopts = {Video::get_video_mode(),
    Textures::get_anisotropic_filtering(),
    Video::get_multisampling(), 
    Video::get_vertical_sync(),
    Textures::get_bilinear_filtering(),
    Textures::get_mipmapping(),
    Video::is_fullscreen(),
    Video::get_screen_width(),
    Video::get_screen_height()};

  {
    ifstream fin("zeniapp.ini");

    string line;
    for(getline(fin, line); fin && line.length(); getline(fin, line)) {
      if(line[0] == '#')
        continue;

      int start = int(line.find('[')), end = int(line.find(']'));
      if(start == -1 || end <= start) {
        cerr << "Bad Section Marker: " << line << endl;
        throw Corrupt_INI();
      }

      string id = line.substr(start + 1, end - start - 1);

      if(id == "Video Options")
        vopts = handle_video_options(fin);
      else
        cerr << "Unrecognized Section Marker: " << line << endl;
    }
  }

  // Start engines
  Video::preinit(vopts.vm, vopts.width, vopts.height, vopts.fullscreen, vopts.multisampling, true);
  if(get_Video().get_vertical_sync() != vopts.vsync)
    get_Video().set_vertical_sync(vopts.vsync);
  Textures::set_texturing_mode(vopts.anisotropy, vopts.bilinear, vopts.mipmapping);
}

/*** main ***/

#include <SDL/SDL_image.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_ttf.h>

#ifndef DISABLE_AL
#include <AL/al.h>
static std::string alErrorString(const ALenum &err) {
  switch(err) {
    case AL_NO_ERROR:          return "AL_NO_ERROR";          break;
    case AL_INVALID_NAME:      return "AL_INVALID_NAME";      break;
    case AL_INVALID_ENUM:      return "AL_INVALID_ENUM";      break;
    case AL_INVALID_VALUE:     return "AL_INVALID_VALUE";     break;
    case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION"; break;
    case AL_OUT_OF_MEMORY:     return "AL_OUT_OF_MEMORY";     break;
    default:                   return "AL_UNKNOWN_ERROR";     break;
  }
}
#endif

static void print_errors() {
  cerr << "Printing all possible error strings:\n";

  cerr << "SDL       : " << (strlen(SDL_GetError()   ) ? SDL_GetError()    : "no error") << endl;
  cerr << "SDL_image : " << (strlen(IMG_GetError ()  ) ? IMG_GetError ()   : "no error") << endl;
  cerr << "SDL_net   : " << (strlen(SDLNet_GetError()) ? SDLNet_GetError() : "no error") << endl;
  cerr << "SDL_ttf   : " << (strlen(TTF_GetError()   ) ? TTF_GetError()    : "no error") << endl;

#ifndef DISABLE_DX9
  //DXGetErrorString?
#endif

#ifndef DISABLE_GL
  cerr << "OpenGL    : " << gluErrorString(glGetError()) << endl;
#endif

#ifndef DISABLE_AL
  cerr << "OpenAL    : " << alErrorString(alGetError()) << endl;
#endif

  cerr << "Cg        : " << cgGetErrorString(cgGetError()) << endl;
}

inline int main2(const int &argc, const char * const argv[]) {
  srand(static_cast<unsigned int>(time(0)));

  vector<string> args(argc - 1);
  for(int i = 1; i < argc; ++i)
    args[i - 1] = argv[i];

  try {
    // Primarily to set up IO redirection
    get_Core();
    
    // Load INI
    load_ini();
    
    // Run Game
    get_Game(&args).run();
  }
  catch(Quit_Event &nonerror) {
    cerr << nonerror.msg << endl;
  }
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4130 )
#endif
  catch(Error &error) {
    cerr << error.msg << endl;

    print_errors();

    assert("Error caught in main - Please see stderr.txt" == 0);
    return 1;
  }
  catch(...) {
    cerr << "Unknown Error (Not of Type 'Zeni::Error')";

    print_errors();

    assert("Unknown Error caught in main" == 0);
    throw;
  }
#ifdef _WINDOWS
#pragma warning( pop )
#endif

  print_errors();

  return 0;
}

int main(int argc, char *argv[]) {
  return main2(argc, argv);
}
