/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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
#include <GL/gl.h>
#endif

namespace Zeni {

  Light::Light(const Color &ambient, const Color &diffuse, const Color &specular, const Point3f &position, const Vector3f &spot_direction)
    : m_light_type(LIGHT_POINT),
    m_diffuse(diffuse),
    m_specular(specular), 
    m_ambient(ambient),
    m_position(position),
    m_spot_direction(spot_direction),
    m_range(pow(2.0f, 100.0f)), 
    m_spot_exponent(1.0f), 
    m_constant_attenuation(1.0f), 
    m_linear_attenuation(0.0f), 
    m_quadratic_attenuation(0.0f), 
    m_spot_theta(2*pi), 
    m_spot_phi(2*pi)
  {
  }

#ifndef DISABLE_GL
  void Light::set(const GLint &number, Video_GL &) const {
    GLfloat position[] = {m_position.x, m_position.y, m_position.z, m_light_type != LIGHT_DIRECTIONAL};

    glEnable(number);
    glLightfv(number, GL_AMBIENT, reinterpret_cast<const GLfloat *>(&m_ambient));
    glLightfv(number, GL_DIFFUSE, reinterpret_cast<const GLfloat *>(&m_diffuse));
    glLightfv(number, GL_SPECULAR, reinterpret_cast<const GLfloat *>(&m_specular));
    glLightfv(number, GL_POSITION, position);
    glLightfv(number, GL_SPOT_DIRECTION, reinterpret_cast<const GLfloat*>(&m_spot_direction));
    glLightf(number, GL_SPOT_EXPONENT, m_spot_exponent);
    glLightf(number, GL_SPOT_CUTOFF, m_spot_phi*90.0f/pi); // halve
    glLightf(number, GL_CONSTANT_ATTENUATION, m_constant_attenuation);
    glLightf(number, GL_LINEAR_ATTENUATION, m_linear_attenuation);
    glLightf(number, GL_QUADRATIC_ATTENUATION, m_quadratic_attenuation);
  }
#endif

#ifndef DISABLE_DX9
  void Light::set(const DWORD &number, Video_DX9 &screen) const {
    screen.get_d3d_device()->SetLight(number, reinterpret_cast<const D3DLIGHT9 *>(this));
    screen.get_d3d_device()->LightEnable(number, TRUE);
  }
#endif

}
