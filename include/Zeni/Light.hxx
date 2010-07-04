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

#ifndef ZENI_LIGHT_HXX
#define ZENI_LIGHT_HXX

#include <Zeni/Light.h>

namespace Zeni {

  void Light::set_light_type(const LIGHT_TYPE &light_type_) {
    light_type = light_type_;
    if(light_type == LIGHT_POINT)
      spot_theta = spot_phi = 2*pi;
  }

  void Light::set_spot_theta(const float &spot_theta_) {
    spot_theta = spot_theta_;

    if(spot_phi < spot_theta)
      set_spot_phi(spot_theta);
  }

  void Light::set_spot_phi(const float &spot_phi_) {
    spot_phi = spot_phi_;

    if(spot_phi == 2*pi)
      light_type = LIGHT_POINT;
    else if(spot_phi < pi)
      light_type = LIGHT_SPOT;

    if(spot_theta > spot_phi)
      spot_theta = spot_phi;
  }

}

#endif
