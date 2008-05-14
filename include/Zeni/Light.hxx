/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
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

#ifndef ZENI_LIGHT_HXX
#define ZENI_LIGHT_HXX

#include <Zeni/Light.h>

namespace Zeni {

  LIGHT_TYPE Light::get_light_type() const {
    return m_light_type;
  }

  const Color & Light::get_ambient() const {
    return m_ambient;
  }

  const Color & Light::get_diffuse() const {
    return m_diffuse;
  }

  const Color & Light::get_specular() const {
    return m_specular;
  }

  const Point3f & Light::get_position() const {
    return m_position;
  }

  const Vector3f & Light::get_spot_direction() const {
    return m_spot_direction;
  }

  float Light::get_range() const {
    return m_range;
  }

  float Light::get_spot_exponent() const {
    return m_spot_exponent;
  }

  float Light::get_constant_attenuation() const {
    return m_constant_attenuation;
  }

  float Light::get_linear_attenuation() const {
    return m_linear_attenuation;
  }

  float Light::get_quadratic_attenuation() const {
    return m_quadratic_attenuation;
  }

  float Light::get_spot_theta() const {
    return m_spot_theta;
  }

  float Light::get_spot_phi() const {
    return m_spot_phi;
  }

  void Light::set_light_type(const LIGHT_TYPE &light_type) {
    m_light_type = light_type;
    if(m_light_type == LIGHT_POINT)
      m_spot_theta = m_spot_phi = 2*pi;
  }

  void Light::set_ambient(const Color &ambient) {m_ambient = ambient;
  }

  void Light::set_diffuse(const Color &diffuse) {m_diffuse = diffuse;
  }

  void Light::set_specular(const Color &specular) {
    m_specular = specular;
  }

  void Light::set_position(const Point3f &position) {
    m_position = position;
  }

  void Light::set_spot_direction(const Vector3f &spot_direction) {
    m_spot_direction = spot_direction;
  }

  void Light::set_range(const float &range) {
    m_range = range;
  }

  void Light::set_spot_exponent(const float &spot_exponent) {
    m_spot_exponent = spot_exponent;
  }

  void Light::set_constant_attenuation(const float &constant_attenuation) {
    m_constant_attenuation = constant_attenuation;
  }

  void Light::set_linear_attenuation(const float &linear_attenuation) {
    m_linear_attenuation = linear_attenuation;
  }

  void Light::set_quadratic_attenuation(const float &quadratic_attenuation) {
    m_quadratic_attenuation = quadratic_attenuation;
  }

  void Light::set_spot_theta(const float &spot_theta) {
    m_spot_theta = spot_theta;

    if(m_spot_phi < m_spot_theta)
      set_spot_phi(m_spot_theta);
  }

  void Light::set_spot_phi(const float &spot_phi) {
    m_spot_phi = spot_phi;

    if(spot_phi == 2*pi)
      m_light_type = LIGHT_POINT;
    else if(m_spot_phi < pi)
      m_light_type = LIGHT_SPOT;

    if(m_spot_theta > m_spot_phi)
      m_spot_theta = m_spot_phi;
  }

}

#endif
