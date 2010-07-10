/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
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
  Play_State() {
    set_pausable(true);
  }

private:
  void on_push() {
    //get_Video().mouse_grab(true);
    get_Video().mouse_hide(true);
  }

  void on_pop() {
    //get_Video().mouse_grab(false);
    get_Video().mouse_hide(false);
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
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text(
#if defined(_WINDOWS)
                   "ALT+F4"
#elif defined(_MACOSX)
                   "Apple+Q"
#else
                   "Ctrl+Q"
#endif
                           " to Quit",
                   Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

namespace Zeni {

  /** Do NOT add any other code to the Zeni namespace unless you intend to modify zenilib.
   *  Modifying the functions below should be the extent of your modifications to zenilib.
   *
   *  If you merely intend to use zenilib, your code should be in a namespace unique
   *  to your game, or in the global namespace.
   *
   *  Access classes and functions in zenilib by prefixing them with Zeni:: in your
   *  header files.  It is acceptable to use 'using' directives in source files only.
   *
   *  All your source files should include <zenilib.h> in the first line, or certain
   *  build targets will give you errors involving precompiled headers.
   */

  Gamestate_One::Gamestate_One(const vector<string> * const args) {
    if(args)
      g_args = *args;

    Video::set_title("zenilib Application");

    get_Video();
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
