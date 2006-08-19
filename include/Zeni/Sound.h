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

/**
 * \class Zeni::BGM_On_Close
 *
 * \ingroup Zenilib
 *
 * \brief A Callback for When the Music Stops
 *
 * If you want something to happen when the music stops playing, create a class 
 * derived from BGM_On_Close to send to Load_BGM.  It could start another song, 
 * or cause something to happen in game.
 *
 * \note If you want looping functionality, it is better to simply leave the play_count at -1.  This will not be called in that case
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Sound
 *
 * \ingroup Zenilib
 *
 * \brief The Sound Singleton
 *
 * The Sound Singleton is responsible for playing music and Sound_Effects.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUND_H
#define ZENI_SOUND_H

#include "Core.h"

#include <SDL/SDL_mixer.h>
#include <string>

struct SDL_Thread;

namespace Zeni {

  class Sound_Effect;

  class BGM_On_Close {
  public:
    virtual ~BGM_On_Close();

    void operator()() {}

    virtual int delay_by() const;
  };

  class Sound {
    friend class BGM_On_Close;

    Sound();
    ~Sound();

    // Undefined
    Sound(const Sound &);
    Sound & operator=(const Sound &);

  public:
    // Get reference to only instance; Might throw Sound_Init_Failure
    static Sound & get_reference();

    // Accessors
    inline bool is_enabled() const; ///< Check Sound is currently enabled

    // Modifiers
    void enable(); ///< Enable Sound
    void disable(); ///< Disable Sound

    // BGM Functions
    void set_BGM(const std::string &filename); ///< Set BackGround Music
    void load_BGM(const int &fade_time = 0, const int &play_count = -1, BGM_On_Close *callback = 0); ///< Load BackGround Music
    void unload_BGM(const int &fade_time = 0); ///< Unload BackGround Music; Any Callback set in load_BGM will not be called in this case.

    // Sound_Effect Functions
    bool play_sound(const Sound_Effect &, const int &loop_times = 0); ///< Play a Sound_Effect

  private:
    std::string m_bgmusic;
    Mix_Music *m_bgmm;
    int m_channels;

    SDL_Thread *m_callback_loop;

    bool m_enabled;
  };

  struct Sound_Init_Failure : public Error {
    Sound_Init_Failure() : Error("Zeni Sound Failed to Initialize Correctly") {}
  };

  struct BGM_Init_Failure : public Error {
    BGM_Init_Failure() : Error("Zeni Sound Failed to Initialize Correctly") {}
  };

}

#ifdef ZENI_INLINES
#include "Sound.hxx"
#endif

#endif
