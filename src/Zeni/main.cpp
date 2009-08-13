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
#include <Zeni/XML.hxx>

#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>

#ifdef _WINDOWS
#include <direct.h>
#else
#include <errno.h>
#endif

#ifdef _MACOSX
#include <mach-o/dyld.h>
#endif

using namespace std;
using namespace Zeni;

static void load_config() {
  XML_Document config_xml("config/zenilib.xml");

  {
    Core::preinit(config_xml["Zenilib"]["Uniqname"].to_string());
    get_Core(); // Primarily to set up IO redirection

    if(config_xml.try_load(get_Core().get_appdata_path() + "config/zenilib.xml"))
      cerr << "User-specific config file loaded from '"
           << get_Core().get_appdata_path() + "config/zenilib.xml"
           << "'." << endl;
    else
      cerr << "User-specific config file not found." << endl;
  }

  XML_Element_c zenilib = config_xml["Zenilib"];

  struct {
    struct {
      int anisotropy;
      bool bilinear_filtering;
      bool mipmapping;
    } textures;

    struct {
      Video_Base::VIDEO_MODE api;
      bool full_screen;
      int multisampling;
      Point2i screen_resolution;
      bool vertical_sync;
    } video;
  } config;

  {
    XML_Element_c textures = zenilib["Textures"];

    config.textures.anisotropy = textures["Anisotropy"].to_int();
    if(config.textures.anisotropy < 0)
      config.textures.anisotropy = 16;

    config.textures.bilinear_filtering = textures["Bilinear_Filtering"].to_bool();

    config.textures.mipmapping = textures["Mipmapping"].to_bool();
  }

  {
    XML_Element_c video = zenilib["Video"];

    const string api = video["API"].to_string();
#ifndef DISABLE_GL
    if(api == "OpenGL")
      config.video.api = Video_Base::ZENI_VIDEO_GL;
    else
#endif
#ifndef DISABLE_DX9
      if(api == "DX9")
        config.video.api = Video_Base::ZENI_VIDEO_DX9;
      else
#endif
      {
        config.video.api = Video_Base::ZENI_VIDEO_ANY;

        if(api == "Disabled")
          Video::set_enabled(false);
      }

    config.video.full_screen = video["Full_Screen"].to_bool();

    config.video.multisampling = video["Multisampling"].to_int();
    if(config.video.multisampling < 0)
      config.video.multisampling = 16;

    {
      XML_Element_c screen_resolution = video["Resolution"];

      config.video.screen_resolution.x = screen_resolution["Width"].to_int();
      config.video.screen_resolution.y = screen_resolution["Height"].to_int();
    }

    config.video.vertical_sync = video["Vertical_Sync"].to_bool();
  }

  // Start engines
  Video::preinit_video_mode(config.video.api);
  Video::preinit_screen_resolution(config.video.screen_resolution);
  Video::preinit_full_screen(config.video.full_screen);
  Video::preinit_multisampling(config.video.multisampling);
  Video::preinit_vertical_sync(config.video.vertical_sync);
  Video::preinit_show_frame(true);

  Textures::set_texturing_mode(config.textures.anisotropy,
                               config.textures.bilinear_filtering,
                               config.textures.mipmapping);
}

/*** main ***/

#ifdef _MACOSX
#include <SDL_image/SDL_image.h>
#include <SDL_net/SDL_net.h>
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_image.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_ttf.h>
#endif

#ifndef DISABLE_AL
#ifdef _MACOSX
#include <ALUT/alut.h>
#else
#include <AL/alut.h>
#endif
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

#ifndef DISABLE_CG
  cerr << "Cg        : " << cgGetErrorString(cgGetError()) << endl;
#endif
}

inline int main2(const size_t &argc, const char * const argv[]) {
  srand(static_cast<unsigned int>(time(0)));

  vector<string> args(argc - 1u);
  for(size_t i = 1u; i < argc; ++i)
    args[i - 1u] = argv[i];

  try {
    // Load config
    load_config();
    
    // Run Game
    get_Game(&args).run();
  }
  catch(Quit_Event &) {
    cerr << "Exiting normally." << endl;
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

// Go up one directory (or strip a file off the end of a directory)
static void up_one_dir(char path[], int &length) {
  for(path[length] = '\0'; length; --length)
    if(path[length] == '/') {
      path[length + 1] = '\0';
      break;
    }
}

int main(int argc, char *argv[]) {
#ifdef _WINDOWS
#ifdef X64
  if(!SetDllDirectory("bin\\x64")) {
#else
  if(!SetDllDirectory("bin")) {
#endif
    cerr << "Setting DLL directory failed with error code ':" << GetLastError() << "'\n";
    return -1;
  }
#else
  {
    char application_path[FILENAME_MAX];
#ifndef _MACOSX
    int length = readlink("/proc/self/exe", application_path, FILENAME_MAX);
#else
    uint32_t size = sizeof(application_path);
    if(_NSGetExecutablePath(application_path, &size)) {
      cerr << "Loading working directory failed.\n";
      return -1;
    }
    int length = strlen(application_path);
    
    for(int i = 0; i != 4; ++i)
#endif
      up_one_dir(application_path, length);
    if(chdir(application_path)) {
      cerr << "chdir: " << application_path << '\n';
      cerr << "Setting working directory failed with error code: '" << errno << "'\n";
      cerr << strerror(errno) << '\n';
      return -1;
    }
  }
#endif

  return main2(size_t(argc), argv);
}
