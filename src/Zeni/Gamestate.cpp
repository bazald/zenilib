/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Gamestate.hxx>

#include <Zeni/Game.hxx>

namespace Zeni {

  void Gamestate_Base::on_event(const SDL_Event &event) {
    switch(event.type) {
    case SDL_ACTIVEEVENT:
      on_active(event.active);
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      on_key(event.key);
      break;
    case SDL_MOUSEMOTION:
      on_mouse_motion(event.motion);
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      on_mouse_button(event.button);
      break;
    case SDL_JOYAXISMOTION:
      on_joy_axis(event.jaxis);
      break;
    case SDL_JOYBALLMOTION:
      on_joy_ball(event.jball);
      break;
    case SDL_JOYHATMOTION:
      on_joy_hat(event.jhat);
      break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      on_joy_button(event.jbutton);
      break;
    case SDL_QUIT:
      on_quit(event.quit);
      break;
    case SDL_SYSWMEVENT:
      on_system_wm_event(event.syswm);
      break;
    case SDL_VIDEORESIZE:
      on_video_resize(event.resize);
      break;
    case SDL_VIDEOEXPOSE:
      on_video_expose(event.expose);
      break;
    case SDL_USEREVENT:
      on_user_event(event.user);
      break;
    default:
      // Impossible
      break;
    }
  }

  void Gamestate_Base::on_key(const SDL_KeyboardEvent &event) {
    if(event.state == SDL_PRESSED)
      if(event.keysym.sym == SDLK_F4 && (event.keysym.mod & KMOD_ALT))
        throw Quit_Event();
      else if(event.keysym.sym == SDLK_ESCAPE)
        Game::get_reference().pop_state();
  }

  void Gamestate_Base::on_mouse_motion(const SDL_MouseMotionEvent &) {
  }

  void Gamestate_Base::on_mouse_button(const SDL_MouseButtonEvent &) {
  }

  void Gamestate_Base::on_joy_axis(const SDL_JoyAxisEvent &) {
  }

  void Gamestate_Base::on_joy_ball(const SDL_JoyBallEvent &) {
  }

  void Gamestate_Base::on_joy_hat(const SDL_JoyHatEvent &) {
  }

  void Gamestate_Base::on_joy_button(const SDL_JoyButtonEvent &) {
  }

  void Gamestate_Base::on_quit(const SDL_QuitEvent &) {
    throw Quit_Event();
  }

  void Gamestate_Base::on_system_wm_event(const SDL_SysWMEvent &) {
  }

  void Gamestate_Base::on_active(const SDL_ActiveEvent &event) {
    if(event.gain) {
      SDL_WM_GrabInput(SDL_GRAB_ON);
      SDL_ShowCursor(false);
    }
    else {
      SDL_WM_GrabInput(SDL_GRAB_OFF);
      SDL_ShowCursor(true);
    }
  }

  void Gamestate_Base::on_video_resize(const SDL_ResizeEvent &) {
    //Video::get_reference().reinit(event.w, event.h, false, true);
  }

  void Gamestate_Base::on_video_expose(const SDL_ExposeEvent &) {
  }

  void Gamestate_Base::on_user_event(const SDL_UserEvent &) {
  }

  char Gamestate_Base::to_char(const SDL_keysym &ks) {
    if((ks.mod & KMOD_CAPS) ^ (ks.mod & KMOD_SHIFT))
      switch(ks.sym) {
      case SDLK_a: return 'A';
      case SDLK_b: return 'B';
      case SDLK_c: return 'C';
      case SDLK_d: return 'D';
      case SDLK_e: return 'E';
      case SDLK_f: return 'F';
      case SDLK_g: return 'G';
      case SDLK_h: return 'H';
      case SDLK_i: return 'I';
      case SDLK_j: return 'J';
      case SDLK_k: return 'K';
      case SDLK_l: return 'L';
      case SDLK_m: return 'M';
      case SDLK_n: return 'N';
      case SDLK_o: return 'O';
      case SDLK_p: return 'P';
      case SDLK_q: return 'Q';
      case SDLK_r: return 'R';
      case SDLK_s: return 'S';
      case SDLK_t: return 'T';
      case SDLK_u: return 'U';
      case SDLK_v: return 'V';
      case SDLK_w: return 'W';
      case SDLK_x: return 'X';
      case SDLK_y: return 'Y';
      case SDLK_z: return 'Z';
      default: break;
    }
    else
      switch(ks.sym) {
      case SDLK_a: return 'a';
      case SDLK_b: return 'b';
      case SDLK_c: return 'c';
      case SDLK_d: return 'd';
      case SDLK_e: return 'e';
      case SDLK_f: return 'f';
      case SDLK_g: return 'g';
      case SDLK_h: return 'h';
      case SDLK_i: return 'i';
      case SDLK_j: return 'j';
      case SDLK_k: return 'k';
      case SDLK_l: return 'l';
      case SDLK_m: return 'm';
      case SDLK_n: return 'n';
      case SDLK_o: return 'o';
      case SDLK_p: return 'p';
      case SDLK_q: return 'q';
      case SDLK_r: return 'r';
      case SDLK_s: return 's';
      case SDLK_t: return 't';
      case SDLK_u: return 'u';
      case SDLK_v: return 'v';
      case SDLK_w: return 'w';
      case SDLK_x: return 'x';
      case SDLK_y: return 'y';
      case SDLK_z: return 'z';
      default: break;
    }

    if(ks.mod & KMOD_NUM)
      switch(ks.sym) {
      case SDLK_KP0:         return '0';
      case SDLK_KP1:         return '1';
      case SDLK_KP2:         return '2';
      case SDLK_KP3:         return '3';
      case SDLK_KP4:         return '4';
      case SDLK_KP5:         return '5';
      case SDLK_KP6:         return '6';
      case SDLK_KP7:         return '7';
      case SDLK_KP8:         return '8';
      case SDLK_KP9:         return '9';
      case SDLK_KP_MULTIPLY: return '*';
      case SDLK_KP_PLUS:     return '+';
      case SDLK_KP_MINUS:    return '-';
      case SDLK_KP_PERIOD:   return '.';
      case SDLK_KP_DIVIDE:   return '/';
      case SDLK_KP_EQUALS:   return '=';
      case SDLK_KP_ENTER:    return '\n';
      default: break;
    }

    switch(ks.sym) {
    case SDLK_BACKSPACE:    return '\b';
    case SDLK_TAB:          return '\t';
    case SDLK_RETURN:       return '\r';
    case SDLK_ESCAPE:       return 0x1B;
    case SDLK_SPACE:        return ' ';
    case SDLK_EXCLAIM:      return '!';
    case SDLK_QUOTEDBL:     return '"';
    case SDLK_HASH:         return '#';
    case SDLK_DOLLAR:       return '$';
    case SDLK_AMPERSAND:    return '&';
    case SDLK_QUOTE:        return '\'';
    case SDLK_LEFTPAREN:    return '(';
    case SDLK_RIGHTPAREN:   return ')';
    case SDLK_ASTERISK:     return '*';
    case SDLK_PLUS:         return '+';
    case SDLK_COMMA:        return ',';
    case SDLK_MINUS:        return '-';
    case SDLK_PERIOD:       return '.';
    case SDLK_SLASH:        return '/';
    case SDLK_0:            return '0';
    case SDLK_1:            return '1';
    case SDLK_2:            return '2';
    case SDLK_3:            return '3';
    case SDLK_4:            return '4';
    case SDLK_5:            return '5';
    case SDLK_6:            return '6';
    case SDLK_7:            return '7';
    case SDLK_8:            return '8';
    case SDLK_9:            return '9';
    case SDLK_COLON:        return ':';
    case SDLK_SEMICOLON:    return ';';
    case SDLK_LESS:         return '<';
    case SDLK_EQUALS:       return '=';
    case SDLK_GREATER:      return '>';
    case SDLK_QUESTION:     return '?';
    case SDLK_AT:           return '@';
    case SDLK_LEFTBRACKET:  return '[';
    case SDLK_BACKSLASH:    return '\\';
    case SDLK_RIGHTBRACKET: return ']';
    case SDLK_CARET:        return '^';
    case SDLK_UNDERSCORE:   return '_';
    case SDLK_BACKQUOTE:    return '`';
    case SDLK_DELETE:       return 0x7F;
    default: break;
    }

    return '\0';
  }

  Gamestate::~Gamestate() {
    m_state->decrement();
  }

  void Gamestate::on_event(const SDL_Event &event) {
    m_state->on_event(event);
  }

  void Gamestate::perform_logic() {
    m_state->perform_logic();
  }

  Zeni_Input_ID::Zeni_Input_ID(const Uint8 &type_, const int &subid_, const int &which_)
    : type(type_),
    subid(subid_),
    which(which_)
  {
  }

  bool Zeni_Input_ID::operator<(const Zeni_Input_ID &rhs) const {
    return type < rhs.type ||
           type == rhs.type && (subid < rhs.subid ||
           subid == rhs.subid && which < rhs.which);
  }

  Gamestate_II::Gamestate_II()
    : m_min_confidence(0.05f),
    m_max_confidence(1.0f)
  {
  }

  void Gamestate_II::on_event(const SDL_Event &event) {
    Zeni_Input_ID event_ID;
    float confidence = 0.0f;
    bool absolute = true;

    switch(event.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      event_ID.type = SDL_KEYDOWN;
      event_ID.subid = event.key.keysym.sym;

      confidence = event.key.state == SDL_PRESSED ? 1.0f : 0.0f;
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      event_ID.type = SDL_MOUSEBUTTONDOWN;
      event_ID.subid = event.button.button;

      confidence = event.button.state == SDL_PRESSED ? 1.0f : 0.0f;
      break;
    case SDL_JOYAXISMOTION:
      event_ID.type = SDL_JOYAXISMOTION;
      event_ID.subid = event.jaxis.axis;
      event_ID.which = event.jaxis.which;

      confidence = (float(event.jaxis.value) + 0.5f) / 32767.5f;
      absolute = false;
      break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      event_ID.type = SDL_JOYBUTTONDOWN;
      event_ID.subid = event.jbutton.button;
      event_ID.which = event.jbutton.which;

      confidence = event.jbutton.state == SDL_PRESSED ? 1.0f : 0.0f;
      break;
    default:
      Gamestate_Base::on_event(event);
      return;
    }

    if(!absolute)
      if(confidence > 0.0f) {
        confidence -= m_min_confidence;
        if(confidence < 0.0f)
          confidence = 0.0f;
        else
          confidence /= m_max_confidence - m_min_confidence;
        if(confidence > 1.0f)
          confidence = 1.0f;
      }
      else if(confidence < 0.0f) {
        confidence += m_min_confidence;
        if(confidence > 0.0f)
          confidence = 0.0f;
        else
          confidence /= m_max_confidence - m_min_confidence;
        if(confidence < -1.0f)
          confidence = -1.0f;
      }

    on_event(event_ID, confidence, get_action(event_ID));
  }

  void Gamestate_II::on_event(const Zeni_Input_ID &, const float &, const int &) {
  }

  int Gamestate_II::get_action(const Zeni_Input_ID &event) {
    std::map<Zeni_Input_ID, int>::const_iterator it = m_ii.find(event);
    if(it != m_ii.end())
      return it->second;
    return 0;
  }

  Zeni_Input_ID Gamestate_II::get_event(const int &action) {
    std::map<int, Zeni_Input_ID>::const_iterator it = m_rii.find(action);
    if(it != m_rii.end())
      return it->second;
    return Zeni_Input_ID();
  }

  void Gamestate_II::set_action(const Zeni_Input_ID &event, const int &action) {
    m_ii[event] = action;
    m_rii[action] = event;
  }

}
