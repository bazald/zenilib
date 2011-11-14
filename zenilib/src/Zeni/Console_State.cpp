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

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  Console_State::Console_State()
    : m_virtual_screen(Point2f(0.0f, 0.0f), Point2f(float(get_Window().get_width() * 600.0f / get_Window().get_height()), 600.0f)),
    m_projector(m_virtual_screen),
    m_bg(Vertex2f_Color(Point2f(0.0f, 0.0f), get_Colors()["console_border"]),
         Vertex2f_Color(Point2f(0.0f, 54.0f + 7.0f * get_Fonts()["system_36_x600"].get_text_height()), get_Colors()["console_border"]),
         Vertex2f_Color(Point2f(m_virtual_screen.second.x, 54.0f + 7.0f * get_Fonts()["system_36_x600"].get_text_height()), get_Colors()["console_border"]),
         Vertex2f_Color(Point2f(m_virtual_screen.second.x, 0.0f), get_Colors()["console_border"])),
    m_log(Point2f(18.0f, 18.0f),
          Point2f(m_virtual_screen.second.x - 18.0f,
                  18.0f + 6.0f * get_Fonts()["system_36_x600"].get_text_height()),
          "system_36_x600",
          "",
          get_Colors()["console_foreground"],
          false),
    m_prompt(Point2f(18.0f, 36.0f + 6.0f * get_Fonts()["system_36_x600"].get_text_height()),
             Point2f(m_virtual_screen.second.x - 18.0f,
                     36.0f + 7.0f * get_Fonts()["system_36_x600"].get_text_height()),
             "system_36_x600",
             "",
             get_Colors()["console_foreground"],
             true),
    m_log_dirty(false),
    m_child(0)
  {
    m_functions["args"] = new Console_Function;

    m_log.give_BG_Renderer(new Widget_Renderer_Color(get_Colors()["console_background"]));
    m_prompt.give_BG_Renderer(new Widget_Renderer_Color(get_Colors()["console_background"]));
  }

  Console_State::~Console_State() {
    for(std::map<String, Console_Function *>::iterator it = m_functions.begin(); it != m_functions.end(); ++it)
      delete it->second;
  }

  Gamestate & Console_State::get_child() {
    assert(m_child);
    return *m_child;
  }

  void Console_State::set_child(Gamestate child) {
    delete m_child;
    m_child = 0;
    m_child = new Gamestate(child);
  }

  void Console_State::clear_child() {
    delete m_child;
    m_child = 0;
  }

  void Console_State::write_to_log(const String &text) {
    if(!m_text.empty())
      m_text += '\n';
    m_text += text;

    const unsigned int max_lines = static_cast<unsigned int>(m_log.get_max_lines());
    for(unsigned int endl_count = 0u, pos = static_cast<unsigned int>(m_text.size() - 1u); pos != static_cast<unsigned int>(-1); --pos)
      if(m_text[pos] == '\n' && ++endl_count == max_lines) {
        m_text = m_text.substr(pos + 1);
        break;
      }

    m_log_dirty = true;
  }

  void Console_State::clear_log() {
    m_text.clear();
    m_log_dirty = true;
  }

  void Console_State::give_function(const String &name, Console_Function * const &function) {
    assert(function);

    if(m_functions[name])
      delete m_functions[name];
    m_functions[name] = function;
  }

  void Console_State::fire_command(const String &text) {
    if(text.empty())
      return;

    String command;
    std::vector<String> args;

    String token;
    bool in_quotes = false;
    bool escaped = false;
    for(String::const_iterator it = text.begin(); it != text.end(); ++it) {
      if(escaped) {
        token += *it;
        escaped = false;
      }
      else {
        switch(*it) {
          case '"':
            if(in_quotes || !token.empty()) {
              if(command.empty())
                command = token;
              else
                args.push_back(token);
              token.clear();
            }
            in_quotes = !in_quotes;
            break;

          case ' ':
          case '\t':
            if(in_quotes)
              token += *it;
            else if(!token.empty()) {
              if(command.empty())
                command = token;
              else
                args.push_back(token);
              token.clear();
            }
            break;

          case '\\':
            escaped = true;
            break;

          default:
            token += *it;
            break;
        }
      }
    }

    if(!token.empty())
      if(command.empty())
        command = token;
      else
        args.push_back(token);
    else if(command.empty())
      return;

    std::map<String, Console_Function *>::iterator func = m_functions.find(command);
    if(func != m_functions.end())
      (*(func->second))(*this, command, args);
    else
      write_to_log("Unrecognized command: " + command);
  }

  void Console_State::on_event(const SDL_Event &event) {
    m_handled_event = false;
    Gamestate_Base::on_event(event);
    if(!m_handled_event && m_child) {
      assert(m_child);
      m_child->on_event(event);
    }
  }

  void Console_State::on_key(const SDL_KeyboardEvent &event) {
    Game &gr = get_Game();
    const bool mod_nonshift = gr.get_key_state(SDLK_LALT) || gr.get_key_state(SDLK_RALT) ||
                              gr.get_key_state(SDLK_LCTRL) || gr.get_key_state(SDLK_RCTRL) ||
#if SDL_VERSION_ATLEAST(1,3,0)
                              gr.get_key_state(SDLK_LGUI) || gr.get_key_state(SDLK_LGUI);
#else
                              gr.get_key_state(SDLK_LMETA) || gr.get_key_state(SDLK_RMETA) ||
                              gr.get_key_state(SDLK_LSUPER) || gr.get_key_state(SDLK_RSUPER);
#endif

    if(event.type == SDL_KEYDOWN &&
       !mod_nonshift)
    {
      switch(event.keysym.sym) {
        case SDLK_RETURN:
          if(m_prompt.get_edit_pos() != -1) {
            fire_command(m_prompt.get_text());
            m_prompt.set_text("");
          }
          break;

        default:
          if(m_prompt.get_edit_pos() != -1) {
            const String backup_text = m_prompt.get_text();
            const int backup_pos = m_prompt.get_edit_pos();
            const bool restore = m_prompt.get_num_lines() == 1;

            m_prompt.on_event(event);

            if(m_prompt.get_num_lines() > 1 && restore) {
              m_prompt.set_text(backup_text);
              m_prompt.seek(backup_pos);
            }
          }
          break;
      }
    }

    m_handled_event = m_prompt.get_edit_pos() != -1;
  }

  void Console_State::on_mouse_button(const SDL_MouseButtonEvent &event) {
    m_prompt.on_event(event, m_virtual_screen);
    m_handled_event = (float(event.y) / get_Window().get_height()) < (m_bg.c.position.y / 600.0f);
  }

  void Console_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
    m_prompt.on_event(event, m_virtual_screen);
    m_handled_event = (float(event.y) / get_Window().get_height()) < (m_bg.c.position.y / 600.0f);
  }

  void Console_State::perform_logic() {
    m_virtual_screen = std::make_pair(Point2f(0.0f, 0.0f), Point2f(float(get_Window().get_width() * 600.0f / get_Window().get_height()), 600.0f));
    m_projector = Projector2D(m_virtual_screen);

    if(m_bg.c.position.x != m_virtual_screen.second.x) {
      m_bg.c.position.x = m_virtual_screen.second.x;
      m_bg.d.position.x = m_virtual_screen.second.x;

      const float right = m_virtual_screen.second.x - 18.0f;
      m_log.set_lower_right(Point2f(right, m_log.get_lower_right().y));
      m_prompt.set_lower_right(Point2f(right, m_prompt.get_lower_right().y));
    }

    m_child->perform_logic();
  }

  void Console_State::prerender() {
    assert(m_child);

    m_child->prerender();

    if(m_log_dirty) {
      m_log.set_text(m_text);

      const size_t num_lines = m_log.get_num_lines();
      const size_t max_lines = m_log.get_max_lines();
      if(num_lines > max_lines)
        m_log.erase_lines(0, static_cast<unsigned int>(num_lines - max_lines));

      m_text = m_log.get_text();
      m_log_dirty = false;
    }
  }

  void Console_State::render() {
    assert(m_child);

    m_child->render();

    Video &vr = get_Video();

    const bool ztest = vr.is_ztest_enabled();
    if(ztest)
      vr.set_ztest(false);

    vr.set_2d(m_virtual_screen);

    vr.render(m_bg);
    m_log.render();
    m_prompt.render();

    if(ztest)
      vr.set_ztest(true);
  }

  void Console_Function::operator()(Console_State &console,
                                    const String &name,
                                    const std::vector<String> &args)
  {
    String fargs;

    if(!args.empty()) {
      std::vector<String>::const_iterator it = args.begin();
      fargs += *it;
      for(++it; it != args.end(); ++it)
        fargs += ", " + *it;
    }

    console.write_to_log(name + '(' + fargs + ')');
  }

}
