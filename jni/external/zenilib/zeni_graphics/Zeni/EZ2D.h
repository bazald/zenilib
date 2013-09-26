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

#ifndef ZENI_EZ2D_H
#define ZENI_EZ2D_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/String.h>
#include <Zeni/Vector3f.h>

#include <Zeni/Define.h>

namespace Zeni {

  /**
   * Render an image in 2D by specifying the coordinates of its corners in screenspace.
   * (0,0) is upper_left, (width, height) is lower_right.
   *
   * \note If lighting is enabled, then odd effects may result if ambient_lighting is set to anything other than the default.
   */
  ZENI_GRAPHICS_DLL void render_image(
    const String &image_name,
    const Point2f &upper_left,
    const Point2f &lower_right,
    const bool &horizontally_flipped = false,
    const Color &color_filter = Color());

  /**
   * Render an image with rotation and scaling.
   * 
   * This works identically to the function with fewer arguments but it takes the
   * extra step of rotating around and scaling away from a given point.
   * 
   * \note If lighting is enabled, then odd effects may result if ambient_lighting is set to anything other than the default.
   */
  ZENI_GRAPHICS_DLL void render_image(
    const String &image_name,
    const Point2f &upper_left,
    const Point2f &lower_right,
    const float &radians_ccw,
    const float &scaling_factor,
    const Point2f &about,
    const bool &horizontally_flipped = false,
    const Color &color_filter = Color());

  /**
   * Check to see if an image_name refers to a Sprite.
   */
  ZENI_GRAPHICS_DLL bool is_sprite(
    const String &image_name);

  /**
   * Find out how many frames a Sprite has.
   */
  ZENI_GRAPHICS_DLL int sprite_num_frames(
    const String &image_name);

  /**
   * Set the current_frame for a Sprite.
   */
  ZENI_GRAPHICS_DLL void set_sprite_frame(
    const String &image_name,
    const int &frame_number);

  /**
   * Increment the current_frame for a Sprite, with wrap-around.
   */
  ZENI_GRAPHICS_DLL void increment_sprite_frame(
    const String &image_name);

  /**
   * Decrement the current_frame for a Sprite, with wrap-around.
   */
  ZENI_GRAPHICS_DLL void decrement_sprite_frame(
    const String &image_name);

}

#include <Zeni/Undefine.h>

#endif
