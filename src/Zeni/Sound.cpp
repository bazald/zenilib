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

#include <Zeni/Sound.hxx>
#include <Zeni/Sounds.hxx>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

using std::string;

namespace Zeni {

  Sound::Sound()
    : m_bgmm(0),
    m_channels(8),
    m_enabled(false)
  {
    // Ensure Core is initialized
    Core::get_reference();

    enable();
  }

  Sound::~Sound() {
    disable();
  }

  Sound & Sound::get_reference() {
    static Sound e_sound;
    return e_sound;
  }

  void Sound::enable() {
    m_enabled = true;

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
      throw Sound_Init_Failure();

    Mix_AllocateChannels(m_channels);
  }

  void Sound::disable() {
    m_enabled = false;

    stop_BGM();
    Mix_CloseAudio();
  }

  void Sound::set_BGM(const string &filename) {
    m_bgmusic = filename;
    
    // Actually load
    
    if(!m_enabled)
      enable();

    stop_BGM();

    if(!m_bgmusic.length())
      throw BGM_Init_Failure();

    if(m_bgmm)
      Mix_FreeMusic(m_bgmm);

    m_bgmm = Mix_LoadMUS(m_bgmusic.c_str());
    if(!m_bgmm)
      throw BGM_Init_Failure();
  }

  void Sound::play_BGM(const int &loops, const int &fade_for_ms, const double &start_second) {
    Mix_HaltMusic();
    Mix_RewindMusic();
    Mix_FadeInMusicPos(m_bgmm, loops, fade_for_ms, start_second);
  }

  bool Sound::play_sound(const Sound_Effect &sound_effect, const int &loop_times) {
    if(!m_enabled)
      enable();

    int channels = Sound::m_channels;
    while(Mix_Playing(-1) < (channels >> 1))
      channels = channels >> 1;
    while(Mix_Playing(-1) >= channels)
      channels = channels << 1;

    if(channels != Sound::m_channels)
      Mix_AllocateChannels(Sound::m_channels = channels);

    Mix_PlayChannelTimed(-1, const_cast<Mix_Chunk *>(sound_effect.get_Mix_Chunk()), loop_times, -1);

    return true;
  }

}
