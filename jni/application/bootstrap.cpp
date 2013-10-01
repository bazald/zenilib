/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_Base {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State() {
    set_pausable(true);
  }

private:
  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    //get_Game().controller_mouse.enabled = false;
  }

  void on_pop() {
    //get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
    //get_Game().controller_mouse.enabled = true;
    for(int i = 0; i != 4; ++i)
      get_Controllers().set_vibration(i, 0.0f, 0.0f);
  }

  void on_cover() {
    for(int i = 0; i != 4; ++i)
      get_Controllers().set_vibration(i, 0.0f, 0.0f);
  }

  void on_controller_axis(const SDL_ControllerAxisEvent &event) {
    if(event.value < -16000 || event.value > 16000)
      get_Game().write_to_console("Axis: " + ulltoa(event.which) + ':' + ulltoa(event.axis));
    if(event.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
      vibration[event.which].first = event.value / 32767.0f;
    if(event.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
      vibration[event.which].second = event.value / 32767.0f;
    get_Controllers().set_vibration(event.which, vibration[event.which].first, vibration[event.which].second);
  }

  void on_controller_button(const SDL_ControllerButtonEvent &event) {
    if(event.state == SDL_PRESSED)
      get_Game().write_to_console("Button: " + ulltoa(event.which) + ':' + ulltoa(event.button));
  }

  void perform_logic() {
    //get_Game().write_to_console("Num Controllers = " + ulltoa(get_Controllers().get_num_controllers()));
  }
  
  std::map<int, std::pair<float, float> > vibration;
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

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("zenilib Application");

      get_Controllers();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().controller_mouse.enabled = true;

      return new Title_State<Play_State, Instructions_State>("Zenipex Library\nApplication");
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
