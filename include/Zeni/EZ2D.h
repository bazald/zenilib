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

#ifndef ZENI_EZ2D_H
#define ZENI_EZ2D_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>

#include <string>

namespace Zeni {

  /**
   * Render an image in 2D by specifying the coordinates of its corners in screenspace.
   * (0,0) is upper_left, (width, height) is lower_right.
   *
   * \note The color_filter will only be applied if lighting is enabled, and then odd effects may result if ambient_lighting is set to anything other than the default.
   */
  void render_image(
    const std::string &image_name,
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
   * \note The color_filter will only be applied if lighting is enabled, and then odd effects may result if ambient_lighting is set to anything other than the default.
   */
  void render_image(
    const std::string &image_name,
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
  bool is_sprite(
    const std::string &image_name);

  /**
   * Find out how many frames a Sprite has.
   */
  int sprite_num_frames(
    const std::string &image_name);

  /**
   * Set the current_frame for a Sprite.
   */
  void set_sprite_frame(
    const std::string &image_name,
    const int &frame_number);

  /**
   * Increment the current_frame for a Sprite, with wrap-around.
   */
  void increment_sprite_frame(
    const std::string &image_name);

  /**
   * Decrement the current_frame for a Sprite, with wrap-around.
   */
  void decrement_sprite_frame(
    const std::string &image_name);

  /**
   * Play a sound effect.
   */
  void play_sound(
    const std::string &sound_name);

}

#endif
