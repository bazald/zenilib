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

  BGM_On_Close::~BGM_On_Close() {
    Sound::get_reference().m_callback_loop = 0;
    delete this;
  }

  int BGM_On_Close::delay_by() const {
    return 500;
  }

  Sound::Sound()
    : m_bgmm(0),
    m_channels(8),
    m_callback_loop(0),
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

    unload_BGM(0);
    Mix_CloseAudio();
  }

  void Sound::set_BGM(const string &filename) {
    m_bgmusic = filename;
  }

  static int bcm_callback_loop(void *callback) {
    BGM_On_Close *callback_fcn = reinterpret_cast<BGM_On_Close *>(callback);

    while(SDL_GetAudioStatus() != SDL_AUDIO_STOPPED)
      SDL_Delay(callback_fcn->delay_by());

    (*callback_fcn)();
    delete callback_fcn;

    return 0;
  }

  void Sound::load_BGM(const int &fade_time, const int &play_count, BGM_On_Close *callback) {
    if(!m_enabled)
      enable();

    unload_BGM(0);

    if(!m_bgmusic.length())
      throw BGM_Init_Failure();

    if(m_bgmm)
      Mix_FreeMusic(m_bgmm);

    m_bgmm = Mix_LoadMUS(m_bgmusic.c_str());
    if(!m_bgmm)
      throw BGM_Init_Failure();

    Mix_FadeInMusic(m_bgmm, play_count, fade_time);

    if(play_count != -1 && callback)
      m_callback_loop = SDL_CreateThread(bcm_callback_loop, callback);
  }

  void Sound::unload_BGM(const int &fade_time) {
    if(!m_enabled)
      return;

    if(m_callback_loop) {
      SDL_KillThread(m_callback_loop);
      m_callback_loop = 0;
    }

    Mix_FadeOutMusic(fade_time);
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
