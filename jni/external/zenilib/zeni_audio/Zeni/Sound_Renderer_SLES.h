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

#ifndef ZENI_SOUND_RENDERER_SLES_H
#define ZENI_SOUND_RENDERER_SLES_H

#include <Zeni/Sound_Renderer.h>

#ifdef ENABLE_SLES

#include <SLES/OpenSLES.h>
#ifdef ANDROID
#include <SLES/OpenSLES_Android.h>
#endif

namespace Zeni {

  class ZENI_AUDIO_DLL Sound_Renderer_SLES : public Sound_Renderer {
    friend class Sound;

    Sound_Renderer_SLES();
    ~Sound_Renderer_SLES();

    // Undefined
    Sound_Renderer_SLES(const Sound_Renderer_SLES &);
    Sound_Renderer_SLES & operator=(const Sound_Renderer_SLES &);

  public:
#ifdef ENABLE_SLES
    SLEngineItf & get_engineEngine();
    SLObjectItf & get_outputMixObject();
#endif

    // Listener Functions
    void set_listener_position(const Point3f &position); ///< Set the position of the listener and BGM.
    void set_listener_velocity(const Vector3f &velocity); ///< Set the velocity of the listener and BGM for the doppler effect.
    void set_listener_forward_and_up(const Vector3f &forward, const Vector3f &up); ///< Set the orientation of the listener
    void set_listener_gain(const float &gain); ///< Set the listener gain

    std::pair<Vector3f, Vector3f> get_listener_forward_and_up() const; ///< Set the orientation of the listener

  private:
    // engine interfaces
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    String m_bgmusic;
    Sound_Buffer *m_bgm;
    Sound_Source *m_bgm_source;
  };

}

#endif

#endif
