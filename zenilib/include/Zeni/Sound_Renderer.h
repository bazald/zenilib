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
 * \class Zeni::Sound
 *
 * \ingroup zenilib
 *
 * \brief The Sound Singleton
 *
 * The Sound Singleton is responsible for initializing and uninitializing OpenAL.
 * It also controls a single Sound_Source that is positioned at the listener's 
 * current location so as to allow the playing of background music.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUND_RENDERER_H
#define ZENI_SOUND_RENDERER_H

#include <Zeni/Coordinate.h>
#include <Zeni/Sound_Buffer.h>

#include <Zeni/Define.h>

namespace Zeni {

  class ZENI_AUDIO_DLL Sound_Renderer {
  public:
    virtual ~Sound_Renderer() {}

    virtual void set_listener_position(const Point3f &/*position*/) {} ///< Set the position of the listener and BGM.
    virtual void set_listener_velocity(const Vector3f &/*velocity*/) {} ///< Set the velocity of the listener and BGM for the doppler effect.
    virtual void set_listener_forward_and_up(const Vector3f &/*forward*/, const Vector3f &/*up*/) {} ///< Set the orientation of the listener
    virtual void set_listener_gain(const float &/*gain*/) {} ///< Set the listener gain

    virtual Point3f get_listener_position() const {return Point3f();} ///< Get the position of the listener and BGM.
    virtual Vector3f get_listener_velocity() const {return Vector3f();} ///< Get the velocity of the listener and BGM.
    virtual std::pair<Vector3f, Vector3f> get_listener_forward_and_up() const {return std::make_pair(ZENI_DEFAULT_FORWARD_VECTOR, ZENI_DEFAULT_UP_VECTOR);} ///< Set the orientation of the listener
  };

  class Sound_Renderer_NULL : public Sound_Renderer {};

}

#include <Zeni/Undefine.h>

#endif
