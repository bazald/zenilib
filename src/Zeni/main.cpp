/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Configurator_Video.h>
#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Sound.hxx>
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

static bool load_config() {
  Zeni::XML_Document config_xml("config/zenilib.xml");
  bool user_config = true;

  {
    Zeni::Core::preinit(config_xml["Zenilib"]["Uniqname"].to_string());
    Zeni::Core &cr = Zeni::get_Core(); // Partially to set up IO redirection

    if(config_xml.try_load(cr.get_appdata_path() + "config/zenilib.xml"))
      std::cerr << "User-specific config file loaded from '"
                << cr.get_appdata_path() + "config/zenilib.xml"
                << "'." << std::endl;
    else {
      std::cerr << "User-specific config file not found." << std::endl;
      user_config = false;
    }
  }

  Zeni::XML_Element_c zenilib = config_xml["Zenilib"];

  struct
#ifndef _MSC_VER
    __attribute__ ((visibility("hidden")))
#endif
  {
    struct {
      int anisotropy;
      bool bilinear_filtering;
      bool mipmapping;
    } textures;

    struct {
      Zeni::Video_Base::VIDEO_MODE api;
      bool full_screen;
      int multisampling;
      Zeni::Point2i screen_resolution;
      bool vertical_sync;
    } video;
  } config;

  {
    Zeni::XML_Element_c textures = zenilib["Textures"];

    config.textures.anisotropy = textures["Anisotropy"].to_int();
    if(config.textures.anisotropy < 0)
      config.textures.anisotropy = 16;

    config.textures.bilinear_filtering = textures["Bilinear_Filtering"].to_bool();

    config.textures.mipmapping = textures["Mipmapping"].to_bool();
  }

  {
    Zeni::XML_Element_c video = zenilib["Video"];

    const std::string api = video["API"].to_string();
#ifndef DISABLE_GL
    if(api == "OpenGL")
      config.video.api = Zeni::Video_Base::ZENI_VIDEO_GL;
    else
#endif
#ifndef DISABLE_DX9
      if(api == "DX9")
        config.video.api = Zeni::Video_Base::ZENI_VIDEO_DX9;
      else
#endif
      {
        config.video.api = Zeni::Video_Base::ZENI_VIDEO_ANY;

        if(api == "Disabled")
          Zeni::Video::set_enabled(false);
      }

    config.video.full_screen = video["Full_Screen"].to_bool();

    config.video.multisampling = video["Multisampling"].to_int();
    if(config.video.multisampling < 0)
      config.video.multisampling = 16;

    {
      Zeni::XML_Element_c screen_resolution = video["Resolution"];

      config.video.screen_resolution.x = screen_resolution["Width"].to_int();
      config.video.screen_resolution.y = screen_resolution["Height"].to_int();
    }

    config.video.vertical_sync = video["Vertical_Sync"].to_bool();
  }

  // Start engines
  Zeni::Video::preinit_video_mode(config.video.api);
  Zeni::Video::preinit_screen_resolution(config.video.screen_resolution);
  Zeni::Video::preinit_full_screen(config.video.full_screen);
  Zeni::Video::preinit_multisampling(config.video.multisampling);
  Zeni::Video::preinit_vertical_sync(config.video.vertical_sync);
  Zeni::Video::preinit_show_frame(true);

  Zeni::Textures::set_texturing_mode(config.textures.anisotropy,
                                     config.textures.bilinear_filtering,
                                     config.textures.mipmapping);

  return user_config;
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
#if defined(_WINDOWS)
#include <al.h>
#elif defined(_MACOSX)
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
#endif

static void print_errors() {
  std::cerr << "Printing all possible error strings:" << std::endl;

  std::cerr << "SDL       : " << (strlen(SDL_GetError()   ) ? SDL_GetError()    : "no error") << std::endl;
  std::cerr << "SDL_image : " << (strlen(IMG_GetError ()  ) ? IMG_GetError ()   : "no error") << std::endl;
  std::cerr << "SDL_net   : " << (strlen(SDLNet_GetError()) ? SDLNet_GetError() : "no error") << std::endl;
  std::cerr << "SDL_ttf   : " << (strlen(TTF_GetError()   ) ? TTF_GetError()    : "no error") << std::endl;

#ifndef DISABLE_DX9
  //DXGetErrorString?
#endif

#ifndef DISABLE_GL
  std::cerr << "OpenGL    : " << gluErrorString(glGetError()) << std::endl;
#endif

#ifndef DISABLE_AL
  std::cerr << "OpenAL    : " << Zeni::alErrorString(alGetError()) << std::endl;
#endif

#ifndef DISABLE_CG
  std::cerr << "Cg        : " << cgGetErrorString(cgGetError()) << std::endl;
#endif
}

inline int main2(const size_t &argc, const char * const argv[]) {
  std::srand(static_cast<unsigned int>(std::time(0)));

  std::vector<std::string> args(argc - 1u);
  for(size_t i = 1u; i < argc; ++i)
    args[i - 1u] = argv[i];

  try {
    // Load config
    const bool user_config = load_config();

    // Initialize Game
    Zeni::Game &gr = Zeni::get_Game(&args);

    // Check Rendering Options on Firstrun
    if(!user_config && Zeni::Video::is_enabled())
      gr.push_state(new Zeni::Configurator_Video::Check_State(true));

    // Run Game
    gr.run();
  }
  catch(Zeni::Quit_Event &) {
    std::cerr << "Exiting normally." << std::endl;
  }
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4130 )
#endif
  catch(Zeni::Error &error) {
    std::cerr << error.msg << std::endl;

    print_errors();

    assert("Zeni::Error caught in main - Please see stderr.txt" == 0);
    return 1;
  }
  catch(std::exception &except) {
    std::cerr << except.what() << std::endl;

    print_errors();

    assert("std::exception caught in main - Please see stderr.txt" == 0);
    return 1;
  }
  catch(...) {
    std::cerr << "Unknown Error (Neither Zeni::Error nor std::exception)";

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
  if(_chdir("..\\..")) {
#else
  if(_chdir("..")) {
#endif
    std::cerr << "Setting working directory failed with error code ':" << GetLastError() << "'\n";
    return -1;
  }
#ifdef X64
  if(!SetDllDirectory("bin\\x64")) {
#else
  if(!SetDllDirectory("bin")) {
#endif
    std::cerr << "Setting DLL directory failed with error code ':" << GetLastError() << "'\n";
    return -2;
  }
#else
  {
    char application_path[FILENAME_MAX];
#ifndef _MACOSX
    int length = readlink("/proc/self/exe", application_path, FILENAME_MAX);
    up_one_dir(application_path, length);
#else
    uint32_t size = sizeof(application_path);
    if(_NSGetExecutablePath(application_path, &size)) {
      std::cerr << "Loading working directory failed.\n";
      return -1;
    }
    int length = strlen(application_path);
    
    for(int i = 0; i != 2; ++i)
      up_one_dir(application_path, length);
    memcpy(application_path + length, "/zenilib", 9);
#endif
    if(chdir(application_path)) {
      std::cerr << "chdir: " << application_path << '\n';
      std::cerr << "Setting working directory failed with error code: '" << errno << "'\n";
      std::cerr << strerror(errno) << '\n';
      return -1;
    }
  }
#endif

  return main2(size_t(argc), argv);
}
