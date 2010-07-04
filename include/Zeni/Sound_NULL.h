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

#ifndef ZENI_SOUND_NULL_H
#define ZENI_SOUND_NULL_H

#include <Zeni/Sound.h>

namespace Zeni {

  class Sound_NULL : public Sound {
    // Get reference to only instance;
    friend Sound & get_Sound(); ///< Get access to the singleton.

    Sound_NULL();
    ~Sound_NULL();

    // Undefined
    Sound_NULL(const Sound_NULL &);
    Sound_NULL & operator=(const Sound_NULL &);

  public:
    // Listener Functions
    inline void set_listener_position_impl(const Point3f &position); ///< Set the position of the listener and BGM.
    inline void set_listener_velocity_impl(const Vector3f &velocity); ///< Set the velocity of the listener and BGM for the doppler effect.
    inline void set_listener_forward_and_up_impl(const Vector3f &forward, const Vector3f &up); ///< Set the orientation of the listener

    inline Point3f get_listener_position_impl() const; ///< Get the position of the listener and BGM.
    inline Vector3f get_listener_velocity_impl() const; ///< Get the velocity of the listener and BGM.
    inline std::pair<Vector3f, Vector3f> get_listener_forward_and_up_impl() const; ///< Set the orientation of the listener
  };

}

#endif
