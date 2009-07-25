/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
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

#include <algorithm>
#include <cmath>

#include <Zeni/Global.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

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
    if(event.state == SDL_PRESSED && event.keysym.sym == SDLK_ESCAPE) {
      Game &game = get_Game();

      if(game.size() > 1u)
        game.pop_state();
    }
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

  void Gamestate_Base::on_active(const SDL_ActiveEvent &/*event*/) {
    //if(event.gain) {
    //  SDL_WM_GrabInput(SDL_GRAB_ON);
    //  SDL_ShowCursor(false);
    //}
    //else {
    //  SDL_WM_GrabInput(SDL_GRAB_OFF);
    //  SDL_ShowCursor(true);
    //}
  }

  void Gamestate_Base::on_video_resize(const SDL_ResizeEvent &) {
    //get_Video().reinit(event.w, event.h, false, true);
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
    //case SDLK_BACKSPACE:    return '\b';
    case SDLK_TAB:          return '\t';
    case SDLK_RETURN:       return '\n'; // '\r' is less sane, but another possibility
    //case SDLK_ESCAPE:       return 0x1B;
    case SDLK_SPACE:        return ' ';
    case SDLK_EXCLAIM:      return '!';
    case SDLK_QUOTEDBL:     return '"';
    case SDLK_HASH:         return '#';
    case SDLK_DOLLAR:       return '$';
    case SDLK_AMPERSAND:    return '&';
    case SDLK_QUOTE:        return (ks.mod & KMOD_SHIFT) ? '"' : '\'';
    case SDLK_LEFTPAREN:    return '(';
    case SDLK_RIGHTPAREN:   return ')';
    case SDLK_ASTERISK:     return '*';
    case SDLK_PLUS:         return '+';
    case SDLK_COMMA:        return (ks.mod & KMOD_SHIFT) ? '<' : ',';
    case SDLK_MINUS:        return (ks.mod & KMOD_SHIFT) ? '_' : '-';
    case SDLK_PERIOD:       return (ks.mod & KMOD_SHIFT) ? '>' : '.';
    case SDLK_SLASH:        return (ks.mod & KMOD_SHIFT) ? '?' : '/';
    case SDLK_0:            return (ks.mod & KMOD_SHIFT) ? ')' : '0';
    case SDLK_1:            return (ks.mod & KMOD_SHIFT) ? '!' : '1';
    case SDLK_2:            return (ks.mod & KMOD_SHIFT) ? '@' : '2';
    case SDLK_3:            return (ks.mod & KMOD_SHIFT) ? '#' : '3';
    case SDLK_4:            return (ks.mod & KMOD_SHIFT) ? '$' : '4';
    case SDLK_5:            return (ks.mod & KMOD_SHIFT) ? '%' : '5';
    case SDLK_6:            return (ks.mod & KMOD_SHIFT) ? '^' : '6';
    case SDLK_7:            return (ks.mod & KMOD_SHIFT) ? '&' : '7';
    case SDLK_8:            return (ks.mod & KMOD_SHIFT) ? '*' : '8';
    case SDLK_9:            return (ks.mod & KMOD_SHIFT) ? '(' : '9';
    case SDLK_COLON:        return ':';
    case SDLK_SEMICOLON:    return (ks.mod & KMOD_SHIFT) ? ':' : ';';
    case SDLK_LESS:         return '<';
    case SDLK_EQUALS:       return (ks.mod & KMOD_SHIFT) ? '+' : '=';
    case SDLK_GREATER:      return '>';
    case SDLK_QUESTION:     return '?';
    case SDLK_AT:           return '@';
    case SDLK_LEFTBRACKET:  return (ks.mod & KMOD_SHIFT) ? '{' : '[';
    case SDLK_BACKSLASH:    return (ks.mod & KMOD_SHIFT) ? '|' : '\\';
    case SDLK_RIGHTBRACKET: return (ks.mod & KMOD_SHIFT) ? '}' : ']';
    case SDLK_CARET:        return '^';
    case SDLK_UNDERSCORE:   return '_';
    case SDLK_BACKQUOTE:    return (ks.mod & KMOD_SHIFT) ? '~' : '`';
    //case SDLK_DELETE:       return 0x7F;
    default: break;
    }

    return '\0';
  }

  SDLKey Gamestate_Base::to_sym(const std::string &text_version) {
#define SC(name) if(text_version == #name) return name;
SC(SDLK_BACKSPACE)
SC(SDLK_TAB)
SC(SDLK_CLEAR)
SC(SDLK_RETURN)
SC(SDLK_PAUSE)
SC(SDLK_ESCAPE)
SC(SDLK_SPACE)
SC(SDLK_EXCLAIM)
SC(SDLK_QUOTEDBL)
SC(SDLK_HASH)
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
SC(SDLK_DELETE)
SC(SDLK_WORLD_0)
SC(SDLK_WORLD_1)
SC(SDLK_WORLD_2)
SC(SDLK_WORLD_3)
SC(SDLK_WORLD_4)
SC(SDLK_WORLD_5)
SC(SDLK_WORLD_6)
SC(SDLK_WORLD_7)
SC(SDLK_WORLD_8)
SC(SDLK_WORLD_9)
SC(SDLK_WORLD_10)
SC(SDLK_WORLD_11)
SC(SDLK_WORLD_12)
SC(SDLK_WORLD_13)
SC(SDLK_WORLD_14)
SC(SDLK_WORLD_15)
SC(SDLK_WORLD_16)
SC(SDLK_WORLD_17)
SC(SDLK_WORLD_18)
SC(SDLK_WORLD_19)
SC(SDLK_WORLD_20)
SC(SDLK_WORLD_21)
SC(SDLK_WORLD_22)
SC(SDLK_WORLD_23)
SC(SDLK_WORLD_24)
SC(SDLK_WORLD_25)
SC(SDLK_WORLD_26)
SC(SDLK_WORLD_27)
SC(SDLK_WORLD_28)
SC(SDLK_WORLD_29)
SC(SDLK_WORLD_30)
SC(SDLK_WORLD_31)
SC(SDLK_WORLD_32)
SC(SDLK_WORLD_33)
SC(SDLK_WORLD_34)
SC(SDLK_WORLD_35)
SC(SDLK_WORLD_36)
SC(SDLK_WORLD_37)
SC(SDLK_WORLD_38)
SC(SDLK_WORLD_39)
SC(SDLK_WORLD_40)
SC(SDLK_WORLD_41)
SC(SDLK_WORLD_42)
SC(SDLK_WORLD_43)
SC(SDLK_WORLD_44)
SC(SDLK_WORLD_45)
SC(SDLK_WORLD_46)
SC(SDLK_WORLD_47)
SC(SDLK_WORLD_48)
SC(SDLK_WORLD_49)
SC(SDLK_WORLD_50)
SC(SDLK_WORLD_51)
SC(SDLK_WORLD_52)
SC(SDLK_WORLD_53)
SC(SDLK_WORLD_54)
SC(SDLK_WORLD_55)
SC(SDLK_WORLD_56)
SC(SDLK_WORLD_57)
SC(SDLK_WORLD_58)
SC(SDLK_WORLD_59)
SC(SDLK_WORLD_60)
SC(SDLK_WORLD_61)
SC(SDLK_WORLD_62)
SC(SDLK_WORLD_63)
SC(SDLK_WORLD_64)
SC(SDLK_WORLD_65)
SC(SDLK_WORLD_66)
SC(SDLK_WORLD_67)
SC(SDLK_WORLD_68)
SC(SDLK_WORLD_69)
SC(SDLK_WORLD_70)
SC(SDLK_WORLD_71)
SC(SDLK_WORLD_72)
SC(SDLK_WORLD_73)
SC(SDLK_WORLD_74)
SC(SDLK_WORLD_75)
SC(SDLK_WORLD_76)
SC(SDLK_WORLD_77)
SC(SDLK_WORLD_78)
SC(SDLK_WORLD_79)
SC(SDLK_WORLD_80)
SC(SDLK_WORLD_81)
SC(SDLK_WORLD_82)
SC(SDLK_WORLD_83)
SC(SDLK_WORLD_84)
SC(SDLK_WORLD_85)
SC(SDLK_WORLD_86)
SC(SDLK_WORLD_87)
SC(SDLK_WORLD_88)
SC(SDLK_WORLD_89)
SC(SDLK_WORLD_90)
SC(SDLK_WORLD_91)
SC(SDLK_WORLD_92)
SC(SDLK_WORLD_93)
SC(SDLK_WORLD_94)
SC(SDLK_WORLD_95)
SC(SDLK_KP0)
SC(SDLK_KP1)
SC(SDLK_KP2)
SC(SDLK_KP3)
SC(SDLK_KP4)
SC(SDLK_KP5)
SC(SDLK_KP6)
SC(SDLK_KP7)
SC(SDLK_KP8)
SC(SDLK_KP9)
SC(SDLK_KP_PERIOD)
SC(SDLK_KP_DIVIDE)
SC(SDLK_KP_MULTIPLY)
SC(SDLK_KP_MINUS)
SC(SDLK_KP_PLUS)
SC(SDLK_KP_ENTER)
SC(SDLK_KP_EQUALS)
SC(SDLK_UP)
SC(SDLK_DOWN)
SC(SDLK_RIGHT)
SC(SDLK_LEFT)
SC(SDLK_INSERT)
SC(SDLK_HOME)
SC(SDLK_END)
SC(SDLK_PAGEUP)
SC(SDLK_PAGEDOWN)
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
SC(SDLK_F13)
SC(SDLK_F14)
SC(SDLK_F15)
SC(SDLK_NUMLOCK)
SC(SDLK_CAPSLOCK)
SC(SDLK_SCROLLOCK)
SC(SDLK_RSHIFT)
SC(SDLK_LSHIFT)
SC(SDLK_RCTRL)
SC(SDLK_LCTRL)
SC(SDLK_RALT)
SC(SDLK_LALT)
SC(SDLK_RMETA)
SC(SDLK_LMETA)
SC(SDLK_LSUPER)
SC(SDLK_RSUPER)
SC(SDLK_MODE)
SC(SDLK_COMPOSE)
SC(SDLK_HELP)
SC(SDLK_PRINT)
SC(SDLK_SYSREQ)
SC(SDLK_BREAK)
SC(SDLK_MENU)
SC(SDLK_POWER)
SC(SDLK_EURO)
SC(SDLK_UNDO)
#undef SC
    return SDLK_UNKNOWN;
  }

//static std::string to_text_part_2(const SDLKey &keysym);

  std::string Gamestate_Base::to_text(const SDLKey &sym) {
    switch(sym) {
#define SC(name) case name: return #name;
SC(SDLK_BACKSPACE)
SC(SDLK_TAB)
SC(SDLK_CLEAR)
SC(SDLK_RETURN)
SC(SDLK_PAUSE)
SC(SDLK_ESCAPE)
SC(SDLK_SPACE)
SC(SDLK_EXCLAIM)
SC(SDLK_QUOTEDBL)
SC(SDLK_HASH)
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
SC(SDLK_DELETE)
SC(SDLK_WORLD_0)
SC(SDLK_WORLD_1)
SC(SDLK_WORLD_2)
SC(SDLK_WORLD_3)
SC(SDLK_WORLD_4)
SC(SDLK_WORLD_5)
SC(SDLK_WORLD_6)
SC(SDLK_WORLD_7)
SC(SDLK_WORLD_8)
SC(SDLK_WORLD_9)
SC(SDLK_WORLD_10)
SC(SDLK_WORLD_11)
SC(SDLK_WORLD_12)
SC(SDLK_WORLD_13)
SC(SDLK_WORLD_14)
SC(SDLK_WORLD_15)
SC(SDLK_WORLD_16)
SC(SDLK_WORLD_17)
SC(SDLK_WORLD_18)
SC(SDLK_WORLD_19)
SC(SDLK_WORLD_20)
SC(SDLK_WORLD_21)
SC(SDLK_WORLD_22)
SC(SDLK_WORLD_23)
SC(SDLK_WORLD_24)
SC(SDLK_WORLD_25)
SC(SDLK_WORLD_26)
SC(SDLK_WORLD_27)
SC(SDLK_WORLD_28)
SC(SDLK_WORLD_29)
SC(SDLK_WORLD_30)
SC(SDLK_WORLD_31)
SC(SDLK_WORLD_32)
SC(SDLK_WORLD_33)
SC(SDLK_WORLD_34)
SC(SDLK_WORLD_35)
SC(SDLK_WORLD_36)
SC(SDLK_WORLD_37)
SC(SDLK_WORLD_38)
SC(SDLK_WORLD_39)
SC(SDLK_WORLD_40)
SC(SDLK_WORLD_41)
SC(SDLK_WORLD_42)
SC(SDLK_WORLD_44)
SC(SDLK_WORLD_45)
SC(SDLK_WORLD_46)
SC(SDLK_WORLD_47)
SC(SDLK_WORLD_48)
SC(SDLK_WORLD_49)
SC(SDLK_WORLD_50)
SC(SDLK_WORLD_51)
SC(SDLK_WORLD_52)
SC(SDLK_WORLD_53)
SC(SDLK_WORLD_54)
SC(SDLK_WORLD_55)
SC(SDLK_WORLD_56)
SC(SDLK_WORLD_57)
SC(SDLK_WORLD_58)
SC(SDLK_WORLD_59)
SC(SDLK_WORLD_60)
SC(SDLK_WORLD_61)
SC(SDLK_WORLD_62)
SC(SDLK_WORLD_63)
SC(SDLK_WORLD_64)
SC(SDLK_WORLD_65)
SC(SDLK_WORLD_66)
SC(SDLK_WORLD_67)
SC(SDLK_WORLD_68)
SC(SDLK_WORLD_69)
SC(SDLK_WORLD_70)
SC(SDLK_WORLD_71)
SC(SDLK_WORLD_72)
SC(SDLK_WORLD_73)
SC(SDLK_WORLD_74)
SC(SDLK_WORLD_75)
SC(SDLK_WORLD_76)
SC(SDLK_WORLD_77)
SC(SDLK_WORLD_78)
SC(SDLK_WORLD_79)
SC(SDLK_WORLD_80)
SC(SDLK_WORLD_81)
SC(SDLK_WORLD_82)
SC(SDLK_WORLD_83)
SC(SDLK_WORLD_84)
SC(SDLK_WORLD_85)
SC(SDLK_WORLD_86)
SC(SDLK_WORLD_87)
SC(SDLK_WORLD_88)
SC(SDLK_WORLD_89)
SC(SDLK_WORLD_90)
SC(SDLK_WORLD_91)
SC(SDLK_WORLD_92)
SC(SDLK_WORLD_93)
SC(SDLK_WORLD_94)
SC(SDLK_WORLD_95)
SC(SDLK_KP0)
SC(SDLK_KP1)
SC(SDLK_KP2)
SC(SDLK_KP3)
SC(SDLK_KP4)
SC(SDLK_KP5)
SC(SDLK_KP6)
SC(SDLK_KP7)
SC(SDLK_KP8)
SC(SDLK_KP9)
SC(SDLK_KP_PERIOD)
SC(SDLK_KP_DIVIDE)
SC(SDLK_KP_MULTIPLY)
SC(SDLK_KP_MINUS)
SC(SDLK_KP_PLUS)
SC(SDLK_KP_ENTER)
SC(SDLK_KP_EQUALS)
SC(SDLK_UP)
SC(SDLK_DOWN)
SC(SDLK_RIGHT)
SC(SDLK_LEFT)
SC(SDLK_INSERT)
SC(SDLK_HOME)
SC(SDLK_END)
SC(SDLK_PAGEUP)
SC(SDLK_PAGEDOWN)
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
SC(SDLK_F13)
SC(SDLK_F14)
SC(SDLK_F15)
SC(SDLK_NUMLOCK)
SC(SDLK_CAPSLOCK)
SC(SDLK_SCROLLOCK)
SC(SDLK_RSHIFT)
SC(SDLK_LSHIFT)
SC(SDLK_RCTRL)
SC(SDLK_LCTRL)
SC(SDLK_RALT)
SC(SDLK_LALT)
SC(SDLK_RMETA)
SC(SDLK_LMETA)
SC(SDLK_LSUPER)
SC(SDLK_RSUPER)
SC(SDLK_MODE)
SC(SDLK_COMPOSE)
SC(SDLK_HELP)
SC(SDLK_PRINT)
SC(SDLK_SYSREQ)
SC(SDLK_BREAK)
SC(SDLK_MENU)
SC(SDLK_POWER)
SC(SDLK_EURO)
SC(SDLK_UNDO)
default: return "SDLK_UNKNOWN";
    }
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
