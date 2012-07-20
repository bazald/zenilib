/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <zeni_rest.h>

#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>

#ifdef _WINDOWS
#include <direct.h>
#else
#include <errno.h>
#include <unistd.h>
#endif

#ifdef _MACOSX
#include <mach-o/dyld.h>
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

static bool load_config() {
  Zeni::XML_Document config_xml("config/zenilib.xml");
  bool user_config = true;

  {
    Zeni::File_Ops::preinit(config_xml["Zenilib"]["Uniqname"].to_string());
    Zeni::File_Ops &fo = Zeni::get_File_Ops(); // Partially to set up IO redirection

    if(config_xml.try_load(fo.get_appdata_path() + "config/zenilib.xml"))
      std::cerr << "User-specific config file loaded from '"
                << fo.get_appdata_path().std_str() + "config/zenilib.xml"
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
      Zeni::Video::VIDEO_MODE api;
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

    const Zeni::String api = video["API"].to_string();
#ifndef DISABLE_GL
    if(api == "OpenGL")
      config.video.api = Zeni::Video::ZENI_VIDEO_GL;
    else
#endif
#ifndef DISABLE_DX9
      if(api == "DX9")
        config.video.api = Zeni::Video::ZENI_VIDEO_DX9;
      else
#endif
      {
        config.video.api = Zeni::Video::ZENI_VIDEO_ANY;

#ifndef ANDROID
        if(api == "Disabled")
          Zeni::Window::set_enabled(false);
#endif
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
#ifndef ANDROID
  Zeni::Window::preinit_resolution(config.video.screen_resolution);
  Zeni::Window::preinit_full_screen(config.video.full_screen);
  Zeni::Window::preinit_show_frame(true);
  Zeni::Video::preinit_video_mode(config.video.api);
#endif
  Zeni::Video::preinit_multisampling(config.video.multisampling);
  Zeni::Video::preinit_vertical_sync(config.video.vertical_sync);

  Zeni::Textures::set_texturing_mode(config.textures.anisotropy,
                                     config.textures.bilinear_filtering,
                                     config.textures.mipmapping);

  return user_config;
}

/*** main ***/

#include <SDL/SDL_net.h>

static void print_errors() {
#ifndef ANDROID
  std::cerr << "Printing all possible error strings:" << std::endl;

  std::cerr << "SDL       : " << (strlen(SDL_GetError()   ) ? SDL_GetError()    : "no error") << std::endl;
  std::cerr << "SDL_net   : " << (strlen(SDLNet_GetError()) ? SDLNet_GetError() : "no error") << std::endl;

  Zeni::Video::print_errors();

#ifndef DISABLE_AL
  std::cerr << "OpenAL    : " << Zeni::Sound_Renderer_AL::errorString() << std::endl;
#endif
#endif
}

inline int main2(const int argc, const char * const * const argv) {
  std::srand(static_cast<unsigned int>(std::time(0)));

  Zeni::g_argc = argc;
  Zeni::g_argv = argv;

  try {
    // Load config
    const bool user_config = load_config();

    // Initialize Game
    Zeni::Game &gr = Zeni::get_Game();

    try {
      // Initialize Gamestate Zero
      if(Zeni::g_gzi)
        gr.push_state((*Zeni::g_gzi)());

#ifndef ANDROID
      // Check Rendering Options on Firstrun
      if(Zeni::Window::is_enabled()) {
        if(!user_config)
          gr.push_state(new Zeni::Configurator_Video::Check_State);
        else
          Zeni::get_Video();
      }
#endif

      // Run Game
      gr.run();
    }
    catch(Zeni::Quit_Event &) {
      print_errors();

      Zeni::Game::completely_destroy();
      //Zeni::Net::completely_destroy();
      Zeni::Fonts::completely_destroy();
      Zeni::Textures::completely_destroy();
      Zeni::Video::completely_destroy();
      Zeni::Window::completely_destroy();
      Zeni::Sound_Source_Pool::completely_destroy();
      Zeni::Sound::completely_destroy();
      Zeni::Sounds::completely_destroy();
      Zeni::Core::completely_destroy();
      Zeni::Joysticks::completely_destroy();
      Zeni::Timer::completely_destroy();
      Zeni::Colors::completely_destroy();
      Zeni::File_Ops::completely_destroy();
      Zeni::Resource::completely_destroy();
      Zeni::Timer_HQ::completely_destroy();

      std::cerr << "Exiting normally." << std::endl;
    }
  }
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4130 )
#endif
  catch(Zeni::Error &error) {
    std::cerr << error.msg << std::endl;

    print_errors();

    Zeni::message_box(error.msg);

    return 1;
  }
  catch(std::exception &except) {
    std::cerr << except.what() << std::endl;

    print_errors();

#ifdef __clang__
    if(strcmp(except.what(), "std::exception"))
#endif
    Zeni::message_box(except.what());

    return 1;
  }
  catch(...) {
    std::cerr << "Unknown Error (Neither Zeni::Error nor std::exception)";

    print_errors();

    Zeni::message_box("Unknown Error caught in main");

    throw;
  }
#ifdef _WINDOWS
#pragma warning( pop )
#endif

  return 0;
}

// Go up one directory (or strip a file off the end of a directory)
#ifndef _WINDOWS
static void up_one_dir(char path[], int &length) {
  for(path[length] = '\0'; length; --length)
    if(path[length] == '/') {
      path[length + 1] = '\0';
      break;
    }
}
#endif

ZENI_REST_DLL int zenilib_main(int argc, char **argv) {
#ifdef _WINDOWS
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

  if(_chdir("..\\..\\assets")) {
    std::cerr << "Setting working directory failed with error code ':" << GetLastError() << "'\n";
    return -1;
  }
#ifdef X64
  if(!SetDllDirectory("bin\\x64")) {
#else
  if(!SetDllDirectory("bin\\x32")) {
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
    int length = int(strlen(application_path));
    
    for(int i = 0; i != 2; ++i)
      up_one_dir(application_path, length);
#endif
    memcpy(application_path + length, "/assets", 8);
    if(chdir(application_path)) {
      std::cerr << "chdir: " << application_path << '\n';
      std::cerr << "Setting working directory failed with error code: '" << errno << "'\n";
      std::cerr << strerror(errno) << '\n';
      return -1;
    }
  }
#endif

  return main2(argc, argv);
}
