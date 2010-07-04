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

#include <zenilib.h> // Otherwise, XCode is unhappy about constructors in Zeni/Global.h

//#include <Zeni/Gamestate.hxx>

//#include <Zeni/Configurator_Video.h>
//#include <Zeni/Game.hxx>
//#include <Zeni/Logo.h>

#include <algorithm>
#include <cmath>

#include <Zeni/Global.h>

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
#if SDL_VERSION_ATLEAST(1,3,0)
    case SDL_MOUSEWHEEL:
      on_mouse_wheel(event.wheel);
      break;
#endif
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
#if SDL_VERSION_ATLEAST(1,3,0)
    case SDL_PROXIMITYIN:
    case SDL_PROXIMITYOUT:
      on_proximity(event.proximity);
      break;
    case SDL_WINDOWEVENT:
      on_window_event(event.window);
      break;
#endif
    case SDL_SYSWMEVENT:
      on_system_wm_event(event.syswm);
      break;
    case SDL_VIDEORESIZE:
      on_video_resize(event.resize);
      break;
#if !SDL_VERSION_ATLEAST(1,3,0)
    case SDL_VIDEOEXPOSE:
      on_video_expose(event.expose);
      break;
#endif
    case SDL_USEREVENT:
      on_user_event(event.user);
      break;
    default:
      // Impossible
      break;
    }
  }

  void Gamestate_Base::on_key(const SDL_KeyboardEvent &event) {
    if(event.state == SDL_PRESSED && event.keysym.sym == SDLK_ESCAPE)
      get_Game().push_state(new Popup_Menu_State(get_Game().pop_state()));
  }

  void Gamestate_Base::on_mouse_motion(const SDL_MouseMotionEvent &) {
  }

  void Gamestate_Base::on_mouse_button(const SDL_MouseButtonEvent &) {
  }

#if SDL_VERSION_ATLEAST(1,3,0)
  void Gamestate_Base::on_mouse_wheel(const SDL_MouseWheelEvent &) {
  }
#endif

  void Gamestate_Base::on_joy_axis(const SDL_JoyAxisEvent &) {
  }

  void Gamestate_Base::on_joy_ball(const SDL_JoyBallEvent &) {
  }

  void Gamestate_Base::on_joy_hat(const SDL_JoyHatEvent &) {
  }

  void Gamestate_Base::on_joy_button(const SDL_JoyButtonEvent &) {
  }

  void Gamestate_Base::on_quit(const SDL_QuitEvent &) {
  }

#if SDL_VERSION_ATLEAST(1,3,0)
  void Gamestate_Base::on_proximity(const SDL_ProximityEvent &) {
  }

  void Gamestate_Base::on_window_event(const SDL_WindowEvent &) {
  }
#endif

  void Gamestate_Base::on_system_wm_event(const SDL_SysWMEvent &) {
  }

  void Gamestate_Base::on_active(const SDL_ActiveEvent &event) {
    if(event.state & SDL_APPINPUTFOCUS) {
      static bool hide_cursor = false;
      static bool grab_input = false;
      Video &vr = get_Video();

      if(event.gain) {
        if(hide_cursor)
          vr.mouse_hide(true);

        if(grab_input)
          vr.mouse_grab(true);
      }
      else {
        hide_cursor = vr.is_mouse_hidden();
        if(hide_cursor)
          vr.mouse_hide(false);

        grab_input = vr.is_mouse_grabbed();
        if(grab_input)
          vr.mouse_grab(false);

        if(m_pausable)
          get_Game().push_state(new Popup_Pause_State(get_Game().pop_state()));
      }
    }
  }

  void Gamestate_Base::on_video_resize(const SDL_ResizeEvent &event) {
    const Point2i resolution (event.w, event.h);
    Video::reinit(&resolution);
    Video::save();
  }

#if !SDL_VERSION_ATLEAST(1,3,0)
  void Gamestate_Base::on_video_expose(const SDL_ExposeEvent &) {
  }
#endif

  void Gamestate_Base::on_user_event(const SDL_UserEvent &) {
  }

  char Gamestate_Base::to_char(const SDL_keysym &ks) {
    Game &gr = get_Game();
    const bool mod_caps = (ks.mod & KMOD_CAPS) != 0;
    const bool mod_shift = gr.get_key_state(SDLK_LSHIFT) || gr.get_key_state(SDLK_RSHIFT);

    if(mod_caps ^ mod_shift)
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
    //case SDLK_BACKSPACE:    return '\b';
    case SDLK_TAB:          return '\t';
    case SDLK_RETURN:       return '\n'; // '\r' is less sane, but another possibility
    //case SDLK_ESCAPE:       return 0x1B;
    case SDLK_SPACE:        return ' ';
    case SDLK_EXCLAIM:      return '!';
    case SDLK_QUOTEDBL:     return '"';
    case SDLK_HASH:         return '#';
#if SDL_VERSION_ATLEAST(1,3,0)
    case SDLK_PERCENT:      return '%';
#endif
    case SDLK_DOLLAR:       return '$';
    case SDLK_AMPERSAND:    return '&';
    case SDLK_QUOTE:        return mod_shift ? '"' : '\'';
    case SDLK_LEFTPAREN:    return '(';
    case SDLK_RIGHTPAREN:   return ')';
    case SDLK_ASTERISK:     return '*';
    case SDLK_PLUS:         return '+';
    case SDLK_COMMA:        return mod_shift ? '<' : ',';
    case SDLK_MINUS:        return mod_shift ? '_' : '-';
    case SDLK_PERIOD:       return mod_shift ? '>' : '.';
    case SDLK_SLASH:        return mod_shift ? '?' : '/';
    case SDLK_0:            return mod_shift ? ')' : '0';
    case SDLK_1:            return mod_shift ? '!' : '1';
    case SDLK_2:            return mod_shift ? '@' : '2';
    case SDLK_3:            return mod_shift ? '#' : '3';
    case SDLK_4:            return mod_shift ? '$' : '4';
    case SDLK_5:            return mod_shift ? '%' : '5';
    case SDLK_6:            return mod_shift ? '^' : '6';
    case SDLK_7:            return mod_shift ? '&' : '7';
    case SDLK_8:            return mod_shift ? '*' : '8';
    case SDLK_9:            return mod_shift ? '(' : '9';
    case SDLK_COLON:        return ':';
    case SDLK_SEMICOLON:    return mod_shift ? ':' : ';';
    case SDLK_LESS:         return '<';
    case SDLK_EQUALS:       return mod_shift ? '+' : '=';
    case SDLK_GREATER:      return '>';
    case SDLK_QUESTION:     return '?';
    case SDLK_AT:           return '@';
    case SDLK_LEFTBRACKET:  return mod_shift ? '{' : '[';
    case SDLK_BACKSLASH:    return mod_shift ? '|' : '\\';
    case SDLK_RIGHTBRACKET: return mod_shift ? '}' : ']';
    case SDLK_CARET:        return '^';
    case SDLK_UNDERSCORE:   return '_';
    case SDLK_BACKQUOTE:    return mod_shift ? '~' : '`';
    //case SDLK_DELETE:       return 0x7F;
    default: break;
    }

    return '\0';
  }

  SDLKey Gamestate_Base::to_sym(const std::string &text_version) {

#define SC(name) if(text_version == #name) return name;

#if SDL_VERSION_ATLEAST(1,3,0)
#define SC0(b)
#define SC1(a) SC(a)
#define SC10(a, b) SC(a)
#else
#define SC0(b) SC(b)
#define SC1(a)
#define SC10(a, b) SC(b)
#endif

SC(SDLK_RETURN)
SC(SDLK_ESCAPE)
SC(SDLK_BACKSPACE)
SC(SDLK_TAB)
SC(SDLK_SPACE)
SC(SDLK_EXCLAIM)
SC(SDLK_QUOTEDBL)
SC(SDLK_HASH)
SC1(SDLK_PERCENT)
SC(SDLK_DOLLAR)
SC(SDLK_AMPERSAND)
SC(SDLK_QUOTE)
SC(SDLK_LEFTPAREN)
SC(SDLK_RIGHTPAREN)
SC(SDLK_ASTERISK)
SC(SDLK_PLUS)
SC(SDLK_COMMA)
SC(SDLK_MINUS)
SC(SDLK_PERIOD)
SC(SDLK_SLASH)
SC(SDLK_0)
SC(SDLK_1)
SC(SDLK_2)
SC(SDLK_3)
SC(SDLK_4)
SC(SDLK_5)
SC(SDLK_6)
SC(SDLK_7)
SC(SDLK_8)
SC(SDLK_9)
SC(SDLK_COLON)
SC(SDLK_SEMICOLON)
SC(SDLK_LESS)
SC(SDLK_EQUALS)
SC(SDLK_GREATER)
SC(SDLK_QUESTION)
SC(SDLK_AT)
SC(SDLK_LEFTBRACKET)
SC(SDLK_BACKSLASH)
SC(SDLK_RIGHTBRACKET)
SC(SDLK_CARET)
SC(SDLK_UNDERSCORE)
SC(SDLK_BACKQUOTE)
SC(SDLK_a)
SC(SDLK_b)
SC(SDLK_c)
SC(SDLK_d)
SC(SDLK_e)
SC(SDLK_f)
SC(SDLK_g)
SC(SDLK_h)
SC(SDLK_i)
SC(SDLK_j)
SC(SDLK_k)
SC(SDLK_l)
SC(SDLK_m)
SC(SDLK_n)
SC(SDLK_o)
SC(SDLK_p)
SC(SDLK_q)
SC(SDLK_r)
SC(SDLK_s)
SC(SDLK_t)
SC(SDLK_u)
SC(SDLK_v)
SC(SDLK_w)
SC(SDLK_x)
SC(SDLK_y)
SC(SDLK_z)

SC(SDLK_CAPSLOCK)

SC(SDLK_F1)
SC(SDLK_F2)
SC(SDLK_F3)
SC(SDLK_F4)
SC(SDLK_F5)
SC(SDLK_F6)
SC(SDLK_F7)
SC(SDLK_F8)
SC(SDLK_F9)
SC(SDLK_F10)
SC(SDLK_F11)
SC(SDLK_F12)

SC10(SDLK_PRINTSCREEN, SDLK_PRINT)
SC(SDLK_SCROLLOCK)
SC(SDLK_PAUSE)
SC(SDLK_INSERT)
SC(SDLK_HOME)
SC(SDLK_PAGEUP)
SC(SDLK_DELETE)
SC(SDLK_END)
SC(SDLK_PAGEDOWN)
SC(SDLK_RIGHT)
SC(SDLK_LEFT)
SC(SDLK_DOWN)
SC(SDLK_UP)

SC10(SDLK_NUMLOCKCLEAR, SDLK_NUMLOCK)
SC(SDLK_KP_DIVIDE)
SC(SDLK_KP_MULTIPLY)
SC(SDLK_KP_MINUS)
SC(SDLK_KP_PLUS)
SC(SDLK_KP_ENTER)
SC10(SDLK_KP_1, SDLK_KP1)
SC10(SDLK_KP_2, SDLK_KP2)
SC10(SDLK_KP_3, SDLK_KP3)
SC10(SDLK_KP_4, SDLK_KP4)
SC10(SDLK_KP_5, SDLK_KP5)
SC10(SDLK_KP_6, SDLK_KP6)
SC10(SDLK_KP_7, SDLK_KP7)
SC10(SDLK_KP_8, SDLK_KP8)
SC10(SDLK_KP_9, SDLK_KP9)
SC10(SDLK_KP_0, SDLK_KP0)
SC(SDLK_KP_PERIOD)

SC10(SDLK_APPLICATION, SDLK_COMPOSE)
SC(SDLK_POWER)
SC(SDLK_KP_EQUALS)
SC(SDLK_F13)
SC(SDLK_F14)
SC(SDLK_F15)
SC1(SDLK_F16)
SC1(SDLK_F17)
SC1(SDLK_F18)
SC1(SDLK_F19)
SC1(SDLK_F20)
SC1(SDLK_F21)
SC1(SDLK_F22)
SC1(SDLK_F23)
SC1(SDLK_F24)
SC1(SDLK_EXECUTE)
SC(SDLK_HELP)
SC(SDLK_MENU)
SC1(SDLK_SELECT)
SC10(SDLK_STOP, SDLK_BREAK)
SC1(SDLK_AGAIN)
SC(SDLK_UNDO)
SC1(SDLK_CUT)
SC1(SDLK_COPY)
SC1(SDLK_PASTE)
SC1(SDLK_FIND)
SC1(SDLK_MUTE)
SC1(SDLK_VOLUMEUP)
SC1(SDLK_VOLUMEDOWN)
SC1(SDLK_KP_COMMA)
SC1(SDLK_KP_EQUALSAS400)

SC1(SDLK_ALTERASE)
SC(SDLK_SYSREQ)
SC1(SDLK_CANCEL)
SC(SDLK_CLEAR)
SC1(SDLK_PRIOR)
SC1(SDLK_RETURN2)
SC1(SDLK_SEPARATOR)
SC1(SDLK_OUT)
SC1(SDLK_OPER)
SC1(SDLK_CLEARAGAIN)
SC1(SDLK_CRSEL)
SC1(SDLK_EXSEL)

SC1(SDLK_KP_00)
SC1(SDLK_KP_000)
SC1(SDLK_THOUSANDSSEPARATOR)
SC1(SDLK_DECIMALSEPARATOR)
SC1(SDLK_CURRENCYUNIT)
SC1(SDLK_CURRENCYSUBUNIT)
SC1(SDLK_KP_LEFTPAREN)
SC1(SDLK_KP_RIGHTPAREN)
SC1(SDLK_KP_LEFTBRACE)
SC1(SDLK_KP_RIGHTBRACE)
SC1(SDLK_KP_TAB)
SC1(SDLK_KP_BACKSPACE)
SC1(SDLK_KP_A)
SC1(SDLK_KP_B)
SC1(SDLK_KP_C)
SC1(SDLK_KP_D)
SC1(SDLK_KP_E)
SC1(SDLK_KP_F)
SC1(SDLK_KP_XOR)
SC1(SDLK_KP_POWER)
SC1(SDLK_KP_PERCENT)
SC1(SDLK_KP_LESS)
SC1(SDLK_KP_GREATER)
SC1(SDLK_KP_AMPERSAND)
SC1(SDLK_KP_DBLAMPERSAND)
SC1(SDLK_KP_VERTICALBAR)
SC1(SDLK_KP_DBLVERTICALBAR)
SC1(SDLK_KP_COLON)
SC1(SDLK_KP_HASH)
SC1(SDLK_KP_SPACE)
SC1(SDLK_KP_AT)
SC1(SDLK_KP_EXCLAM)
SC1(SDLK_KP_MEMSTORE)
SC1(SDLK_KP_MEMRECALL)
SC1(SDLK_KP_MEMCLEAR)
SC1(SDLK_KP_MEMADD)
SC1(SDLK_KP_MEMSUBTRACT)
SC1(SDLK_KP_MEMMULTIPLY)
SC1(SDLK_KP_MEMDIVIDE)
SC1(SDLK_KP_PLUSMINUS)
SC1(SDLK_KP_CLEAR)
SC1(SDLK_KP_CLEARENTRY)
SC1(SDLK_KP_BINARY)
SC1(SDLK_KP_OCTAL)
SC1(SDLK_KP_DECIMAL)
SC1(SDLK_KP_HEXADECIMAL)

SC(SDLK_LCTRL)
SC(SDLK_LSHIFT)
SC(SDLK_LALT)
SC1(SDLK_LGUI)
SC0(SDLK_LMETA)
SC0(SDLK_LSUPER)
SC(SDLK_RCTRL)
SC(SDLK_RSHIFT)
SC(SDLK_RALT)
SC1(SDLK_RGUI)
SC0(SDLK_RMETA)
SC0(SDLK_RSUPER)

SC(SDLK_MODE)

SC1(SDLK_AUDIONEXT)
SC1(SDLK_AUDIOPREV)
SC1(SDLK_AUDIOSTOP)
SC1(SDLK_AUDIOPLAY)
SC1(SDLK_AUDIOMUTE)
SC1(SDLK_MEDIASELECT)
SC1(SDLK_WWW)
SC1(SDLK_MAIL)
SC1(SDLK_CALCULATOR)
SC1(SDLK_COMPUTER)
SC1(SDLK_AC_SEARCH)
SC1(SDLK_AC_HOME)
SC1(SDLK_AC_BACK)
SC1(SDLK_AC_FORWARD)
SC1(SDLK_AC_STOP)
SC1(SDLK_AC_REFRESH)
SC1(SDLK_AC_BOOKMARKS)

SC1(SDLK_BRIGHTNESSDOWN)
SC1(SDLK_BRIGHTNESSUP)
SC1(SDLK_DISPLAYSWITCH)
SC1(SDLK_KBDILLUMTOGGLE)
SC1(SDLK_KBDILLUMDOWN)
SC1(SDLK_KBDILLUMUP)
SC1(SDLK_EJECT)
SC1(SDLK_SLEEP)

SC0(SDLK_EURO)
SC0(SDLK_WORLD_0)
SC0(SDLK_WORLD_1)
SC0(SDLK_WORLD_2)
SC0(SDLK_WORLD_3)
SC0(SDLK_WORLD_4)
SC0(SDLK_WORLD_5)
SC0(SDLK_WORLD_6)
SC0(SDLK_WORLD_7)
SC0(SDLK_WORLD_8)
SC0(SDLK_WORLD_9)
SC0(SDLK_WORLD_10)
SC0(SDLK_WORLD_11)
SC0(SDLK_WORLD_12)
SC0(SDLK_WORLD_13)
SC0(SDLK_WORLD_14)
SC0(SDLK_WORLD_15)
SC0(SDLK_WORLD_16)
SC0(SDLK_WORLD_17)
SC0(SDLK_WORLD_18)
SC0(SDLK_WORLD_19)
SC0(SDLK_WORLD_20)
SC0(SDLK_WORLD_21)
SC0(SDLK_WORLD_22)
SC0(SDLK_WORLD_23)
SC0(SDLK_WORLD_24)
SC0(SDLK_WORLD_25)
SC0(SDLK_WORLD_26)
SC0(SDLK_WORLD_27)
SC0(SDLK_WORLD_28)
SC0(SDLK_WORLD_29)
SC0(SDLK_WORLD_30)
SC0(SDLK_WORLD_31)
SC0(SDLK_WORLD_32)
SC0(SDLK_WORLD_33)
SC0(SDLK_WORLD_34)
SC0(SDLK_WORLD_35)
SC0(SDLK_WORLD_36)
SC0(SDLK_WORLD_37)
SC0(SDLK_WORLD_38)
SC0(SDLK_WORLD_39)
SC0(SDLK_WORLD_40)
SC0(SDLK_WORLD_41)
SC0(SDLK_WORLD_42)
SC0(SDLK_WORLD_43)
SC0(SDLK_WORLD_44)
SC0(SDLK_WORLD_45)
SC0(SDLK_WORLD_46)
SC0(SDLK_WORLD_47)
SC0(SDLK_WORLD_48)
SC0(SDLK_WORLD_49)
SC0(SDLK_WORLD_50)
SC0(SDLK_WORLD_51)
SC0(SDLK_WORLD_52)
SC0(SDLK_WORLD_53)
SC0(SDLK_WORLD_54)
SC0(SDLK_WORLD_55)
SC0(SDLK_WORLD_56)
SC0(SDLK_WORLD_57)
SC0(SDLK_WORLD_58)
SC0(SDLK_WORLD_59)
SC0(SDLK_WORLD_60)
SC0(SDLK_WORLD_61)
SC0(SDLK_WORLD_62)
SC0(SDLK_WORLD_63)
SC0(SDLK_WORLD_64)
SC0(SDLK_WORLD_65)
SC0(SDLK_WORLD_66)
SC0(SDLK_WORLD_67)
SC0(SDLK_WORLD_68)
SC0(SDLK_WORLD_69)
SC0(SDLK_WORLD_70)
SC0(SDLK_WORLD_71)
SC0(SDLK_WORLD_72)
SC0(SDLK_WORLD_73)
SC0(SDLK_WORLD_74)
SC0(SDLK_WORLD_75)
SC0(SDLK_WORLD_76)
SC0(SDLK_WORLD_77)
SC0(SDLK_WORLD_78)
SC0(SDLK_WORLD_79)
SC0(SDLK_WORLD_80)
SC0(SDLK_WORLD_81)
SC0(SDLK_WORLD_82)
SC0(SDLK_WORLD_83)
SC0(SDLK_WORLD_84)
SC0(SDLK_WORLD_85)
SC0(SDLK_WORLD_86)
SC0(SDLK_WORLD_87)
SC0(SDLK_WORLD_88)
SC0(SDLK_WORLD_89)
SC0(SDLK_WORLD_90)
SC0(SDLK_WORLD_91)
SC0(SDLK_WORLD_92)
SC0(SDLK_WORLD_93)
SC0(SDLK_WORLD_94)
SC0(SDLK_WORLD_95)

#undef SC10
#undef SC1
#undef SC0
#undef SC

    return SDLK_UNKNOWN;
  }

//static std::string to_text_part_2(const SDLKey &keysym);

  std::string Gamestate_Base::to_text(const SDLKey &sym) {
    switch(sym) {

#define SC(name) case name: return #name;

#if SDL_VERSION_ATLEAST(1,3,0)
#define SC0(b)
#define SC1(a) SC(a)
#define SC10(a, b) SC(a)
#else
#define SC0(b) SC(b)
#define SC1(a)
#define SC10(a, b) SC(b)
#endif

SC(SDLK_RETURN)
SC(SDLK_ESCAPE)
SC(SDLK_BACKSPACE)
SC(SDLK_TAB)
SC(SDLK_SPACE)
SC(SDLK_EXCLAIM)
SC(SDLK_QUOTEDBL)
SC(SDLK_HASH)
SC1(SDLK_PERCENT)
SC(SDLK_DOLLAR)
SC(SDLK_AMPERSAND)
SC(SDLK_QUOTE)
SC(SDLK_LEFTPAREN)
SC(SDLK_RIGHTPAREN)
SC(SDLK_ASTERISK)
SC(SDLK_PLUS)
SC(SDLK_COMMA)
SC(SDLK_MINUS)
SC(SDLK_PERIOD)
SC(SDLK_SLASH)
SC(SDLK_0)
SC(SDLK_1)
SC(SDLK_2)
SC(SDLK_3)
SC(SDLK_4)
SC(SDLK_5)
SC(SDLK_6)
SC(SDLK_7)
SC(SDLK_8)
SC(SDLK_9)
SC(SDLK_COLON)
SC(SDLK_SEMICOLON)
SC(SDLK_LESS)
SC(SDLK_EQUALS)
SC(SDLK_GREATER)
SC(SDLK_QUESTION)
SC(SDLK_AT)
SC(SDLK_LEFTBRACKET)
SC(SDLK_BACKSLASH)
SC(SDLK_RIGHTBRACKET)
SC(SDLK_CARET)
SC(SDLK_UNDERSCORE)
SC(SDLK_BACKQUOTE)
SC(SDLK_a)
SC(SDLK_b)
SC(SDLK_c)
SC(SDLK_d)
SC(SDLK_e)
SC(SDLK_f)
SC(SDLK_g)
SC(SDLK_h)
SC(SDLK_i)
SC(SDLK_j)
SC(SDLK_k)
SC(SDLK_l)
SC(SDLK_m)
SC(SDLK_n)
SC(SDLK_o)
SC(SDLK_p)
SC(SDLK_q)
SC(SDLK_r)
SC(SDLK_s)
SC(SDLK_t)
SC(SDLK_u)
SC(SDLK_v)
SC(SDLK_w)
SC(SDLK_x)
SC(SDLK_y)
SC(SDLK_z)

SC(SDLK_CAPSLOCK)

SC(SDLK_F1)
SC(SDLK_F2)
SC(SDLK_F3)
SC(SDLK_F4)
SC(SDLK_F5)
SC(SDLK_F6)
SC(SDLK_F7)
SC(SDLK_F8)
SC(SDLK_F9)
SC(SDLK_F10)
SC(SDLK_F11)
SC(SDLK_F12)

SC10(SDLK_PRINTSCREEN, SDLK_PRINT)
SC(SDLK_SCROLLOCK)
SC(SDLK_PAUSE)
SC(SDLK_INSERT)
SC(SDLK_HOME)
SC(SDLK_PAGEUP)
SC(SDLK_DELETE)
SC(SDLK_END)
SC(SDLK_PAGEDOWN)
SC(SDLK_RIGHT)
SC(SDLK_LEFT)
SC(SDLK_DOWN)
SC(SDLK_UP)

SC10(SDLK_NUMLOCKCLEAR, SDLK_NUMLOCK)
SC(SDLK_KP_DIVIDE)
SC(SDLK_KP_MULTIPLY)
SC(SDLK_KP_MINUS)
SC(SDLK_KP_PLUS)
SC(SDLK_KP_ENTER)
SC10(SDLK_KP_1, SDLK_KP1)
SC10(SDLK_KP_2, SDLK_KP2)
SC10(SDLK_KP_3, SDLK_KP3)
SC10(SDLK_KP_4, SDLK_KP4)
SC10(SDLK_KP_5, SDLK_KP5)
SC10(SDLK_KP_6, SDLK_KP6)
SC10(SDLK_KP_7, SDLK_KP7)
SC10(SDLK_KP_8, SDLK_KP8)
SC10(SDLK_KP_9, SDLK_KP9)
SC10(SDLK_KP_0, SDLK_KP0)
SC(SDLK_KP_PERIOD)

SC10(SDLK_APPLICATION, SDLK_COMPOSE)
SC(SDLK_POWER)
SC(SDLK_KP_EQUALS)
SC(SDLK_F13)
SC(SDLK_F14)
SC(SDLK_F15)
SC1(SDLK_F16)
SC1(SDLK_F17)
SC1(SDLK_F18)
SC1(SDLK_F19)
SC1(SDLK_F20)
SC1(SDLK_F21)
SC1(SDLK_F22)
SC1(SDLK_F23)
SC1(SDLK_F24)
SC1(SDLK_EXECUTE)
SC(SDLK_HELP)
SC(SDLK_MENU)
SC1(SDLK_SELECT)
SC10(SDLK_STOP, SDLK_BREAK)
SC1(SDLK_AGAIN)
SC(SDLK_UNDO)
SC1(SDLK_CUT)
SC1(SDLK_COPY)
SC1(SDLK_PASTE)
SC1(SDLK_FIND)
SC1(SDLK_MUTE)
SC1(SDLK_VOLUMEUP)
SC1(SDLK_VOLUMEDOWN)
SC1(SDLK_KP_COMMA)
SC1(SDLK_KP_EQUALSAS400)

SC1(SDLK_ALTERASE)
SC(SDLK_SYSREQ)
SC1(SDLK_CANCEL)
SC(SDLK_CLEAR)
SC1(SDLK_PRIOR)
SC1(SDLK_RETURN2)
SC1(SDLK_SEPARATOR)
SC1(SDLK_OUT)
SC1(SDLK_OPER)
SC1(SDLK_CLEARAGAIN)
SC1(SDLK_CRSEL)
SC1(SDLK_EXSEL)

SC1(SDLK_KP_00)
SC1(SDLK_KP_000)
SC1(SDLK_THOUSANDSSEPARATOR)
SC1(SDLK_DECIMALSEPARATOR)
SC1(SDLK_CURRENCYUNIT)
SC1(SDLK_CURRENCYSUBUNIT)
SC1(SDLK_KP_LEFTPAREN)
SC1(SDLK_KP_RIGHTPAREN)
SC1(SDLK_KP_LEFTBRACE)
SC1(SDLK_KP_RIGHTBRACE)
SC1(SDLK_KP_TAB)
SC1(SDLK_KP_BACKSPACE)
SC1(SDLK_KP_A)
SC1(SDLK_KP_B)
SC1(SDLK_KP_C)
SC1(SDLK_KP_D)
SC1(SDLK_KP_E)
SC1(SDLK_KP_F)
SC1(SDLK_KP_XOR)
SC1(SDLK_KP_POWER)
SC1(SDLK_KP_PERCENT)
SC1(SDLK_KP_LESS)
SC1(SDLK_KP_GREATER)
SC1(SDLK_KP_AMPERSAND)
SC1(SDLK_KP_DBLAMPERSAND)
SC1(SDLK_KP_VERTICALBAR)
SC1(SDLK_KP_DBLVERTICALBAR)
SC1(SDLK_KP_COLON)
SC1(SDLK_KP_HASH)
SC1(SDLK_KP_SPACE)
SC1(SDLK_KP_AT)
SC1(SDLK_KP_EXCLAM)
SC1(SDLK_KP_MEMSTORE)
SC1(SDLK_KP_MEMRECALL)
SC1(SDLK_KP_MEMCLEAR)
SC1(SDLK_KP_MEMADD)
SC1(SDLK_KP_MEMSUBTRACT)
SC1(SDLK_KP_MEMMULTIPLY)
SC1(SDLK_KP_MEMDIVIDE)
SC1(SDLK_KP_PLUSMINUS)
SC1(SDLK_KP_CLEAR)
SC1(SDLK_KP_CLEARENTRY)
SC1(SDLK_KP_BINARY)
SC1(SDLK_KP_OCTAL)
SC1(SDLK_KP_DECIMAL)
SC1(SDLK_KP_HEXADECIMAL)

SC(SDLK_LCTRL)
SC(SDLK_LSHIFT)
SC(SDLK_LALT)
SC1(SDLK_LGUI)
SC0(SDLK_LMETA)
SC0(SDLK_LSUPER)
SC(SDLK_RCTRL)
SC(SDLK_RSHIFT)
SC(SDLK_RALT)
SC1(SDLK_RGUI)
SC0(SDLK_RMETA)
SC0(SDLK_RSUPER)

SC(SDLK_MODE)

SC1(SDLK_AUDIONEXT)
SC1(SDLK_AUDIOPREV)
SC1(SDLK_AUDIOSTOP)
SC1(SDLK_AUDIOPLAY)
SC1(SDLK_AUDIOMUTE)
SC1(SDLK_MEDIASELECT)
SC1(SDLK_WWW)
SC1(SDLK_MAIL)
SC1(SDLK_CALCULATOR)
SC1(SDLK_COMPUTER)
SC1(SDLK_AC_SEARCH)
SC1(SDLK_AC_HOME)
SC1(SDLK_AC_BACK)
SC1(SDLK_AC_FORWARD)
SC1(SDLK_AC_STOP)
SC1(SDLK_AC_REFRESH)
SC1(SDLK_AC_BOOKMARKS)

SC1(SDLK_BRIGHTNESSDOWN)
SC1(SDLK_BRIGHTNESSUP)
SC1(SDLK_DISPLAYSWITCH)
SC1(SDLK_KBDILLUMTOGGLE)
SC1(SDLK_KBDILLUMDOWN)
SC1(SDLK_KBDILLUMUP)
SC1(SDLK_EJECT)
SC1(SDLK_SLEEP)

SC0(SDLK_EURO)
SC0(SDLK_WORLD_0)
SC0(SDLK_WORLD_1)
SC0(SDLK_WORLD_2)
SC0(SDLK_WORLD_3)
SC0(SDLK_WORLD_4)
SC0(SDLK_WORLD_5)
SC0(SDLK_WORLD_6)
SC0(SDLK_WORLD_7)
SC0(SDLK_WORLD_8)
SC0(SDLK_WORLD_9)
SC0(SDLK_WORLD_10)
SC0(SDLK_WORLD_11)
SC0(SDLK_WORLD_12)
SC0(SDLK_WORLD_13)
SC0(SDLK_WORLD_14)
SC0(SDLK_WORLD_15)
SC0(SDLK_WORLD_16)
SC0(SDLK_WORLD_17)
SC0(SDLK_WORLD_18)
SC0(SDLK_WORLD_19)
SC0(SDLK_WORLD_20)
SC0(SDLK_WORLD_21)
SC0(SDLK_WORLD_22)
SC0(SDLK_WORLD_23)
SC0(SDLK_WORLD_24)
SC0(SDLK_WORLD_25)
SC0(SDLK_WORLD_26)
SC0(SDLK_WORLD_27)
SC0(SDLK_WORLD_28)
SC0(SDLK_WORLD_29)
SC0(SDLK_WORLD_30)
SC0(SDLK_WORLD_31)
SC0(SDLK_WORLD_32)
SC0(SDLK_WORLD_33)
SC0(SDLK_WORLD_34)
SC0(SDLK_WORLD_35)
SC0(SDLK_WORLD_36)
SC0(SDLK_WORLD_37)
SC0(SDLK_WORLD_38)
SC0(SDLK_WORLD_39)
SC0(SDLK_WORLD_40)
SC0(SDLK_WORLD_41)
SC0(SDLK_WORLD_42)
SC0(SDLK_WORLD_43)
SC0(SDLK_WORLD_44)
SC0(SDLK_WORLD_45)
SC0(SDLK_WORLD_46)
SC0(SDLK_WORLD_47)
SC0(SDLK_WORLD_48)
SC0(SDLK_WORLD_49)
SC0(SDLK_WORLD_50)
SC0(SDLK_WORLD_51)
SC0(SDLK_WORLD_52)
SC0(SDLK_WORLD_53)
SC0(SDLK_WORLD_54)
SC0(SDLK_WORLD_55)
SC0(SDLK_WORLD_56)
SC0(SDLK_WORLD_57)
SC0(SDLK_WORLD_58)
SC0(SDLK_WORLD_59)
SC0(SDLK_WORLD_60)
SC0(SDLK_WORLD_61)
SC0(SDLK_WORLD_62)
SC0(SDLK_WORLD_63)
SC0(SDLK_WORLD_64)
SC0(SDLK_WORLD_65)
SC0(SDLK_WORLD_66)
SC0(SDLK_WORLD_67)
SC0(SDLK_WORLD_68)
SC0(SDLK_WORLD_69)
SC0(SDLK_WORLD_70)
SC0(SDLK_WORLD_71)
SC0(SDLK_WORLD_72)
SC0(SDLK_WORLD_73)
SC0(SDLK_WORLD_74)
SC0(SDLK_WORLD_75)
SC0(SDLK_WORLD_76)
SC0(SDLK_WORLD_77)
SC0(SDLK_WORLD_78)
SC0(SDLK_WORLD_79)
SC0(SDLK_WORLD_80)
SC0(SDLK_WORLD_81)
SC0(SDLK_WORLD_82)
SC0(SDLK_WORLD_83)
SC0(SDLK_WORLD_84)
SC0(SDLK_WORLD_85)
SC0(SDLK_WORLD_86)
SC0(SDLK_WORLD_87)
SC0(SDLK_WORLD_88)
SC0(SDLK_WORLD_89)
SC0(SDLK_WORLD_90)
SC0(SDLK_WORLD_91)
SC0(SDLK_WORLD_92)
SC0(SDLK_WORLD_93)
SC0(SDLK_WORLD_94)
SC0(SDLK_WORLD_95)

#undef SC10
#undef SC1
#undef SC0
#undef SC

default: return "SDLK_UNKNOWN";
    }
  }

  void Gamestate_Base::render() {
    static Logo logo(Point2f(1.5f, 0.5f), 1.0f, Color(1.0f, 0.875, 0.875, 0.875), Color(1.0f, 0.125, 0.125, 0.175));

    get_Video().set_2d(std::make_pair(Point2f(), Point2f(4.0f, 2.0f)), true);

    logo.render();
  }

  void Gamestate_Base::on_push() {
  }

  void Gamestate_Base::on_pop() {
  }

  Gamestate::~Gamestate() {
    if(m_state)
      m_state->decrement();
  }

  Zeni_Input_ID::Zeni_Input_ID(const Uint8 &type_, const int &subid_, const int &which_)
    : type(type_),
    subid(subid_),
    which(which_),
    previous_confidence(0.0f)
  {
  }

  bool Zeni_Input_ID::operator<(const Zeni_Input_ID &rhs) const {
    return type < rhs.type ||
           (type == rhs.type && (subid < rhs.subid ||
           (subid == rhs.subid && which < rhs.which)));
  }

  Gamestate_II::Gamestate_II()
    : m_joyball_min(ZENI_DEFAULT_II_JOYBALL_MIN),
    m_joyball_max(ZENI_DEFAULT_II_JOYBALL_MAX),
    m_joystick_min(ZENI_DEFAULT_II_JOYSTICK_MIN),
    m_joystick_max(ZENI_DEFAULT_II_JOYSTICK_MAX),
    m_mouse_min(ZENI_DEFAULT_II_MOUSE_MIN),
    m_mouse_max(ZENI_DEFAULT_II_MOUSE_MAX)
  {
  }

  void Gamestate_II::on_event(const SDL_Event &event) {
    switch(event.type) {
    case SDL_JOYAXISMOTION:
      {
        float confidence = (float(event.jaxis.value) + 0.5f) / 32767.5f;
        const float ac = fabs(confidence);
        const float nm = confidence < 0.0f ? -1.0f : 1.0f;
        confidence = nm * std::min(std::max(ac - m_joystick_min, 0.0f) / (m_joystick_max - m_joystick_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_JOYAXISMOTION, event.jaxis.axis, event.jaxis.which), confidence);
      }
      break;
    case SDL_JOYBALLMOTION:
      {
        const int ac = abs(event.jball.xrel);
        const int nm = event.jball.xrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_joyball_min), 0.0f) / (m_joyball_max - m_joyball_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_JOYBALLMOTION, event.jball.ball * 2 + 0, event.jball.which), confidence);
      }
      {
        const int ac = abs(event.jball.yrel);
        const int nm = event.jball.yrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_joyball_min), 0.0f) / (m_joyball_max - m_joyball_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_JOYBALLMOTION, event.jball.ball * 2 + 1, event.jball.which), confidence);
      }
      break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      {
        const float confidence = event.jbutton.state == SDL_PRESSED ? 1.0f : 0.0f;

        fire_event(Zeni_Input_ID(SDL_JOYBUTTONDOWN, event.jbutton.button, event.jbutton.which), confidence);
      }
      break;
    case SDL_JOYHATMOTION:
      {
        float confidence;

        switch(event.jhat.value) {
          case SDL_HAT_LEFT:
          case SDL_HAT_LEFTDOWN:
          case SDL_HAT_LEFTUP:
            confidence = -1.0f;
            break;

          case SDL_HAT_RIGHT:
          case SDL_HAT_RIGHTDOWN:
          case SDL_HAT_RIGHTUP:
            confidence = 1.0f;
            break;

          case SDL_HAT_CENTERED:
          case SDL_HAT_DOWN:
          case SDL_HAT_UP:
          default:
            confidence = 0.0f;
            break;
        }

        fire_event(Zeni_Input_ID(SDL_JOYHATMOTION, 0, event.jhat.which), confidence);
      }
      {
        float confidence;

        switch(event.jhat.value) {
          case SDL_HAT_DOWN:
          case SDL_HAT_LEFTDOWN:
          case SDL_HAT_RIGHTDOWN:
            confidence = -1.0f;
            break;

          case SDL_HAT_UP:
          case SDL_HAT_LEFTUP:
          case SDL_HAT_RIGHTUP:
            confidence = 1.0f;
            break;

          case SDL_HAT_CENTERED:
          case SDL_HAT_LEFT:
          case SDL_HAT_RIGHT:
          default:
            confidence = 0.0f;
            break;
        }

        fire_event(Zeni_Input_ID(SDL_JOYHATMOTION, 1, event.jhat.which), confidence);
      }
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      {
        const float confidence = event.key.state == SDL_PRESSED ? 1.0f : 0.0f;

        fire_event(Zeni_Input_ID(SDL_KEYDOWN, event.key.keysym.sym, 0 /*event.key.which*/), confidence);
      }
      break;
    case SDL_MOUSEMOTION:
      {
        const int ac = abs(event.motion.xrel);
        const int nm = event.motion.xrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_mouse_min), 0.0f) / (m_mouse_max - m_mouse_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_MOUSEMOTION, 0, 0 /*event.motion.which*/), confidence);
      }
      {
        const int ac = abs(event.motion.yrel);
        const int nm = event.motion.yrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_mouse_min), 0.0f) / (m_mouse_max - m_mouse_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_MOUSEMOTION, 1, 0 /*event.motion.which*/), confidence);
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      {
        const float confidence = event.button.state == SDL_PRESSED ? 1.0f : 0.0f;

        fire_event(Zeni_Input_ID(SDL_MOUSEBUTTONDOWN, event.button.button, 0 /*event.button.which*/), confidence);
      }
      break;
    default:
      Gamestate_Base::on_event(event);
      break;
    }
  }

  void Gamestate_II::on_event(const Zeni_Input_ID &id, const float &confidence, const int &) {
    if(id.type == SDL_KEYDOWN && id.subid == SDLK_ESCAPE && confidence == 1.0f)
      get_Game().push_state(new Popup_Menu_State(get_Game().pop_state()));
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

  void Gamestate_II::fire_event(const Zeni_Input_ID &id, const float &confidence) {
    std::map<Zeni_Input_ID, int>::iterator it = m_ii.find(id);
    if(it != m_ii.end()) {
      float &pc = it->first.previous_confidence;
      if(pc != confidence) {
        pc = confidence;
        on_event(id, confidence, it->second);
      }
    }
    else
      on_event(id, confidence, 0);
  }

  bool Quit_Event::fired = false;

}

#include <Zeni/Global_Undef.h>
