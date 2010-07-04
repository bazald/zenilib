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

#include <Zeni/Light.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video_DX9.hxx>

#ifndef DISABLE_GL
#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif
#endif

namespace Zeni {

  Light::Light(const Color &ambient_, const Color &diffuse_, const Color &specular_, const Point3f &position_, const Vector3f &spot_direction_)
    : light_type(LIGHT_POINT),
    diffuse(diffuse_),
    specular(specular_), 
    ambient(ambient_),
    position(position_),
    spot_direction(spot_direction_),
    range(pow(2.0f, 100.0f)), 
    spot_exponent(1.0f), 
    constant_attenuation(1.0f), 
    linear_attenuation(0.0f), 
    quadratic_attenuation(0.0f), 
    spot_theta(2*pi), 
    spot_phi(2*pi)
  {
  }

#ifndef DISABLE_GL
  void Light::set(const GLenum &number, Video_GL &) const {
    GLfloat pos[] = {position.x, position.y, position.z, 1.0f};

    if(light_type == LIGHT_DIRECTIONAL) {
      pos[0] = -spot_direction.x;
      pos[1] = -spot_direction.y;
      pos[2] = -spot_direction.z;
      pos[3] = 0.0f;
    }

    glEnable(number);
    glLightfv(number, GL_AMBIENT, reinterpret_cast<const GLfloat *>(&ambient));
    glLightfv(number, GL_DIFFUSE, reinterpret_cast<const GLfloat *>(&diffuse));
    glLightfv(number, GL_SPECULAR, reinterpret_cast<const GLfloat *>(&specular));
    glLightfv(number, GL_POSITION, pos);
    glLightfv(number, GL_SPOT_DIRECTION, reinterpret_cast<const GLfloat*>(&spot_direction));
    glLightf(number, GL_SPOT_EXPONENT, spot_exponent);
    glLightf(number, GL_SPOT_CUTOFF, spot_phi*90.0f/pi); // halve
    glLightf(number, GL_CONSTANT_ATTENUATION, constant_attenuation);
    glLightf(number, GL_LINEAR_ATTENUATION, linear_attenuation);
    glLightf(number, GL_QUADRATIC_ATTENUATION, quadratic_attenuation);
  }
#endif

#ifndef DISABLE_DX9
  void Light::set(const DWORD &number, Video_DX9 &screen) const {
    screen.get_d3d_device()->SetLight(number, reinterpret_cast<const D3DLIGHT9 *>(this));
    screen.get_d3d_device()->LightEnable(number, TRUE);
  }
#endif

}
