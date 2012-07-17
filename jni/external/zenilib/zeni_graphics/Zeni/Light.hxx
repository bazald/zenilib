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

#ifndef ZENI_LIGHT_HXX
#define ZENI_LIGHT_HXX

#include <Zeni/Light.h>

namespace Zeni {

  void Light::set_light_type(const LIGHT_TYPE &light_type_) {
    light_type = light_type_;
    if(light_type == LIGHT_POINT)
      spot_theta = spot_phi = 2*Global::pi;
  }

  void Light::set_spot_theta(const float &spot_theta_) {
    spot_theta = spot_theta_;

    if(spot_phi < spot_theta)
      set_spot_phi(spot_theta);
  }

  void Light::set_spot_phi(const float &spot_phi_) {
    spot_phi = spot_phi_;

    if(spot_phi == 2*Global::pi)
      light_type = LIGHT_POINT;
    else if(spot_phi < Global::pi)
      light_type = LIGHT_SPOT;

    if(spot_theta > spot_phi)
      spot_theta = spot_phi;
  }

}

#endif
