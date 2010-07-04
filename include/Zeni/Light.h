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

/**
 * \class Zeni::Light
 *
 * \ingroup zenilib
 *
 * \brief An Abstraction of a Light
 *
 * The Light class simply provides Lighting data to both Direct3D9 and OpenGL.
 *
 * \note To OpenGL developers: Direct3D behavior won out over OpenGL behavior. Angles describing the spread of a spotlight should be twice what you're used to.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_LIGHT_H
#define ZENI_LIGHT_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>

#ifndef DISABLE_GL
#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif
#endif

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

namespace Zeni {

  class Video_GL;
  class Video_DX9;

  enum LIGHT_TYPE {LIGHT_POINT = 1, LIGHT_SPOT = 2, LIGHT_DIRECTIONAL = 3};

  struct Light {
    /// An Alternative to the numerous setter functions
    Light(const Color &ambient = Color(1.0f, 0.0f, 0.0f, 0.0f), 
      const Color &diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f), 
      const Color &specular = Color(1.0f, 1.0f, 1.0f, 1.0f), 
      const Point3f &position = Point3f(0.0f, 0.0f, 0.0f), 
      const Vector3f &spot_direction = Vector3f(0.0f, 0.0f, -1.0f));

    // Helpers
    inline void set_light_type(const LIGHT_TYPE &light_type); ///< Set the type of the Light
    inline void set_spot_theta(const float &spot_theta); ///< Set the angle, in radians, describing the size of the inner cone (automatically increases phi if necessary)
    inline void set_spot_phi(const float &spot_phi); ///< Set the angle, in radians, describing the size of the outer cone (automatically decreases theta if necessary)

#ifndef DISABLE_GL
    void set(const GLenum &number, Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    void set(const DWORD &number, Video_DX9 &screen) const;
#endif

    LIGHT_TYPE light_type; ///< The type of the Light
    Color diffuse; ///< The diffuse Color
    Color specular; ///< The specular Color
    Color ambient; ///< The ambient Color
    Point3f position; ///< The position of the Light (irrelevant for directional lights or purely ambient lights)
    Point3f spot_direction; ///< The direction of the Light (relevent only to spotlights)
    float range; ///< The max distance reached by the light; NOT SUPPORTED BY OPENGL
    float spot_exponent; ///< Similar to Falloff in OpenGL, describes the intensity of the Light from the inner cone to the outer cone (Note that the inner cone is size 0 in OpenGL)
    float constant_attenuation; ///< Part of the equation describing the weakening of Light over distance.  Read online for more information.
    float linear_attenuation; ///< Part of the equation describing the weakening of Light over distance.  Read online for more information.
    float quadratic_attenuation; ///< Part of the equation describing the weakening of Light over distance.  Read online for more information.
    float spot_theta; ///< The angle, in radians, describing the size of the inner cone (automatically increases phi if necessary); NOT SUPPORTED BY OPENGL
    float spot_phi; ///< The angle, in radians, describing the size of the outer cone (automatically decreases theta if necessary)
  };

}

#endif
