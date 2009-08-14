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
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State()
  {
  }

private:
  void on_push() {
    //SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(false);
  }

  void on_pop() {
    //SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(true);
  }
};

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text("ALT-F4 to Quit",
                   Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

namespace Zeni {

  Gamestate_One::Gamestate_One(const vector<string> * const args) {
    if(args)
      g_args = *args;

    get_Video().set_title("Zenilib Application");
    get_Textures();
    get_Fonts();
    get_Sounds();
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    // TODO: Parse commandline arguments and bootstrap into titlestate or other gamestate;
 
    Game &gr = get_Game();
    gr.pop_state();
    gr.push_state(new Title_State<Play_State, Instructions_State>("Zenipex Library\nApplication"));
  }

}
