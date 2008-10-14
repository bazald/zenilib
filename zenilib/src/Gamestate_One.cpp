/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include <zenilib.h>

#include <Zeni/Fonts.h>
#include <Zeni/Sounds.h>
#include <Zeni/Textures.h>

#include <Zeni/Sound.hxx>
#include <Zeni/Gamestate_One.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace Zeni;

static vector<string> g_args;

namespace Zeni {

  Gamestate_One::Gamestate_One(const vector<string> * const args) {
#ifdef GRAB_MOUSE
    SDL_WM_GrabInput(SDL_GRAB_ON);
#endif
#ifdef HIDE_MOUSE
    SDL_ShowCursor(0);
#endif
    
    if(args)
      g_args = *args;
  
    Textures::get_reference();
    Fonts::get_reference();
    Sounds::get_reference();
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    //// TODO: Parse commandline arguments and bootstrap into titlestate or other gamestate;
    //
    //Game &gr = Game::get_reference();
    //gr.pop_state();
    //gr.push_state(new MyCustomGamestate());
    //return;
    
    SDL_Delay(5);
  }

}
