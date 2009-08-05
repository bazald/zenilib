/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include <zenilib.h>

using namespace std;
using namespace Zeni;

static vector<string> g_args;

class Play_State : public Gamestate_Base {
  void on_push () {
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(false);
  }

  void on_pop() {
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(true);
  }
};

class Instructions_State : public Gamestate_Base {
  void render() {
    get_Video().set_2d();
    get_Fonts()["system_36_600"].render_text("ALT-F4 to Quit", Point2f(), get_Colors()["title_text"]);
  }
};

namespace Zeni {

  Gamestate_One::Gamestate_One(const vector<string> * const args) {
    if(args)
      g_args = *args;

    get_Textures();
    get_Fonts();
    get_Sounds();
    get_Video().set_title("Zenilib Application");
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    // TODO: Parse commandline arguments and bootstrap into titlestate or other gamestate;
 
    Game &gr = get_Game();
    gr.pop_state();
    gr.push_state(new Title_State<Play_State, Instructions_State>("Long Title:\nSubtitle"));
  }

}
