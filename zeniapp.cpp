/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#define ZENI_INLINES

#include <Zeni/zeniapp.h>
#include <Zeni/Textures.h>
#include <Zeni/Video.h>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Gamestate_One::Gamestate_One(const vector<string> * const) {
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
    if(Video::get_reference().get_vertical_sync() != vopts.vsync)
      Video::get_reference().set_vertical_sync(vopts.vsync);
    Textures::set_texturing_mode(vopts.anisotropy, vopts.bilinear, vopts.mipmapping);

    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(0);
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    SDL_Delay(5); // To prevent this useless example from overloading your CPU
  }

  void Gamestate_One::render() {
  }

  Gamestate_One::Video_Options Gamestate_One::handle_video_options(istream &is) {
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
          vopts.vm = ZENI_VIDEO_GL;
        else
#endif
#ifndef DISABLE_DX9
          if(value == "DX9")
            vopts.vm = ZENI_VIDEO_DX9;
          else
#endif
            vopts.vm = ZENI_VIDEO_ANY;
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

}
