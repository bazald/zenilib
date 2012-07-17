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
 * \class Zeni::Sound_Source_Pool
 *
 * \ingroup zenilib
 *
 * \brief This class manages Sound_Sources.
 *
 * This class assigns Sound_Source_HW to 'Sound_Source's on update.
 *
 * This avoids the problem of users generating more 'Sound_Source's than
 * the hardware on a given computer actually supports.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUND_SOURCE_POOL_H
#define ZENI_SOUND_SOURCE_POOL_H

#include <Zeni/Singleton.h>
#include <Zeni/Sound_Source.h>

#include <vector>

#include <Zeni/Define.h>

namespace Zeni {

  class Sound_Source_Pool;

#ifdef _WINDOWS
  ZENI_AUDIO_EXT template class ZENI_AUDIO_DLL Singleton<Sound_Source_Pool>;
#endif

  class ZENI_AUDIO_DLL Sound_Source_Pool : public Singleton<Sound_Source_Pool> {
    friend class Singleton<Sound_Source_Pool>;
    friend class Sound_Source;
    friend ZENI_AUDIO_DLL void play_sound(
      const String &sound_name,
      const float &pitch,
      const float &gain,
      const Point3f &position,
      const Vector3f &velocity);

    static Sound_Source_Pool * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    Sound_Source_Pool();
    ~Sound_Source_Pool();

    // Undefined
    Sound_Source_Pool(const Sound_Source_Pool &);
    Sound_Source_Pool & operator=(const Sound_Source_Pool &);

  public:
    class ZENI_AUDIO_DLL Replacement_Policy {
    public:
      virtual ~Replacement_Policy() {};

      /// (Default) Priority Sort:  Playing/Not-Playing, Priority, Gain, Recency
      virtual bool operator()(const Sound_Source &lhs, const Sound_Source &rhs) const;

      bool operator()(const Sound_Source * const &lhs, const Sound_Source * const &rhs) const;
    };

    class ZENI_AUDIO_DLL Positional_Replacement_Policy : public Replacement_Policy {
    public:
      Positional_Replacement_Policy(const Point3f &listener_position_);

      /// Positional Priority Sort:  Playing/Not-Playing, Priority, Computed Gain, Recency
      virtual bool operator()(const Sound_Source &lhs, const Sound_Source &rhs) const;

      Point3f listener_position;
    };

    const Replacement_Policy & get_Replacement_Policy() const; ///< Get the Replacement_Policy
    void give_Replacement_Policy(Replacement_Policy * const &replacement_policy); ///< Give the Sound_Source_Pool a new Replacement_Policy (which it will delete later)
    void lend_Replacement_Policy(Replacement_Policy * const &replacement_policy); ///< Give the Sound_Source_Pool a new Replacement_Policy (which it will NEVER delete)

    void pause_all(); ///< Pause all Sound_Sources.
    void unpause_all(); ///< Unpause all paused Sound_Sources.
    void purge(); ///< Purge all Sound_Source_HW and all playing_and_destroying (created by play_sound(...))

    void update(); ///< Redistribute hardware Sound_Sources according to the Replacement_Policy.  Newer Sound_Sources are implicitly prioritized over older Sound_Sources.  (Called automatically)

  private:
    void play_and_destroy(Sound_Source * const &sound_source); ///< Play a Sound_Source and destroy it; Used by play_sound(...)

    void set_Replacement_Policy(Replacement_Policy * const &replacement_policy); ///< Set the Replacement_Policy directly
    void insert_Sound_Source(Sound_Source &sound_source); // on Sound_Source construction
    void remove_Sound_Source(Sound_Source &sound_source); // on Sound_Source destruction

    void destroy_all_hw(); ///< Purge all Sound_Source_HW, but leave playing_and_destroying intact

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::vector<Sound_Source *> m_handles;
    std::vector<Sound_Source *> m_playing_and_destroying;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    
    Replacement_Policy * m_replacement_policy;
    bool delete_m_replacement_policy;

    bool m_muted;
  };

  ZENI_AUDIO_DLL Sound_Source_Pool & get_Sound_Source_Pool(); ///< Get access to the singleton.

  /**
    * Play a sound effect.
    */
  ZENI_AUDIO_DLL void play_sound(
    const String &sound_name,
    const float &pitch = ZENI_DEFAULT_PITCH,
    const float &gain = ZENI_DEFAULT_GAIN,
    const Point3f &position = Point3f(),
    const Vector3f &velocity = Vector3f());

}

#include <Zeni/Undefine.h>

#endif
