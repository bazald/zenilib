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

/**
* \class Zeni::Title_State
*
* \ingroup zenilib
*
* \brief Provide a simple title screen for games
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include <Zeni/Popup_State.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Widget_Gamestate.h>

namespace Zeni {

  template <typename PLAY_STATE, typename INSTRUCTIONS_STATE>
  class Title_State : public Widget_Gamestate {
    Title_State(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);
    Title_State<PLAY_STATE, INSTRUCTIONS_STATE> & operator=(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);

  public:
    class Title : public Text_Box {
      Title(const Title &);
      Title & operator=(const Title &);

    public:
      Title(const String &title_)
        : Text_Box(Point2f(000.0f, 50.0f), Point2f(800.0f, 250.0f),
                   "title", title_, get_Colors()["title_text"])
      {
        give_BG_Renderer(new Widget_Renderer_Color(Color(0.0f, 0.0f, 0.0f, 0.0f)));
      }
    } title;

    class Play_Button : public Text_Button {
      Play_Button(const Play_Button &);
      Play_Button & operator=(const Play_Button &);

    public:
      Play_Button()
        : Text_Button(Point2f(200.0f, 250.0f), Point2f(600.0f, 310.0f),
                      "system_36_800x600", "Play")
      {
      }

      void on_accept() {
        get_Game().push_state(new PLAY_STATE());
      }
    } play_button;

    class Instructions_Button : public Text_Button {
      Instructions_Button(const Instructions_Button &);
      Instructions_Button & operator=(const Instructions_Button &);

    public:
      Instructions_Button()
        : Text_Button(Point2f(200.0f, 330.0f), Point2f(600.0f, 390.0f),
                      "system_36_800x600", "Instructions")
      {
      }

      void on_accept() {
        get_Game().push_state(new INSTRUCTIONS_STATE());
      }
    } instructions_button;

#ifndef ANDROID
    Popup_Menu_State::Configure_Video_Button configure_video_button;
#endif
    Popup_Menu_State::Sound_Check_Box sound_check_box;
    Popup_Menu_State::Quit_Button quit_button;

    Title_State(const String &title_)
      : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
      title(title_),
#ifndef ANDROID
      configure_video_button(Point2f(200.0f, 410.0f), Point2f(600.0f, 470.0f)),
#endif
      sound_check_box(Point2f(200.0f, 490.0f), Point2f(260.0f, 550.0f)),
      quit_button(Point2f(400.0f, 490.0f), Point2f(600.0f, 550.0f))
    {
      m_widgets.lend_Widget(title);
      m_widgets.lend_Widget(play_button);
      m_widgets.lend_Widget(instructions_button);
#ifndef ANDROID
      m_widgets.lend_Widget(configure_video_button);
#endif
      m_widgets.lend_Widget(sound_check_box);
      m_widgets.lend_Widget(quit_button);

      title.set_justify(ZENI_CENTER);

      get_Video().set_clear_Color(get_Colors()["title_bg"]);
    }

    ~Title_State() {
      get_Video().set_clear_Color(Color(1.0f, 0.0f, 0.0f, 0.0f));
    }

#ifndef ANDROID
    void on_key(const SDL_KeyboardEvent &/*event*/)
    {
    }
#endif

    void perform_logic() {
      Widget_Gamestate::perform_logic();
      get_Video().set_clear_Color(get_Colors()["title_bg"]);
    }

  };

}

#endif
