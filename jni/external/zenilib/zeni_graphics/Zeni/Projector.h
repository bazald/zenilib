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
 * \class Zeni::Projector
 *
 * \ingroup zenilib
 *
 * \brief Projector Base Class
 *
 * A base class for Projector2D and Projector3D
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Projector2D
 *
 * \ingroup zenilib
 *
 * \brief The 2D Projector
 *
 * For converting between world and screen coordinates in 2D
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Projector3D
 *
 * \ingroup zenilib
 *
 * \brief The 3D Projector
 *
 * For converting between world and screen coordinates in 3D
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_PROJECTOR_H
#define ZENI_PROJECTOR_H

#include <Zeni/Camera.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Video.h>

namespace Zeni {

  class ZENI_GRAPHICS_DLL Projector {
  protected:
    inline Projector(const std::pair<Point2i, Point2i> &viewport = std::make_pair(Point2i(), get_Video().get_render_target_size()));
    inline ~Projector();

    inline void init(const std::pair<Point2i, Point2i> &viewport);

    inline const Vector3f & offset() const;
    inline const Vector3f & size() const;

  private:
    Vector3f m_offset;
    Vector3f m_size;
  };

  class ZENI_GRAPHICS_DLL Projector2D : public Projector {
  public:
    inline Projector2D(
      const std::pair<Point2f, Point2f> &camera2d = std::make_pair(Point2f(0.0f, 0.0f), Point2f(float(get_Video().get_render_target_size().x), float(get_Video().get_render_target_size().y))),
      const std::pair<Point2i, Point2i> &viewport = std::make_pair(Point2i(), get_Video().get_render_target_size()));

    inline Vector3f project(const Vector3f &world_coord) const; ///< Map coordinates in the viewing area to screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [-1, 1])
    inline Point3f project(const Point3f &world_coord) const; ///< Map coordinates in the viewing area to screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [-1, 1])
    inline Point2f project(const Point2f &world_coord) const; ///< Map coordinates in the viewing area to screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [-1, 1])

    inline Vector3f unproject(const Vector3f &screen_coord) const; ///< Map screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [-1, 1]) to coordinates in the viewing area
    inline Point3f unproject(const Point3f &screen_coord) const; ///< Map screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [-1, 1]) to coordinates in the viewing area
    inline Point2f unproject(const Point2f &screen_coord) const; ///< Map screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [-1, 1]) to coordinates in the viewing area

  private:
    inline void init(
      const std::pair<Point2f, Point2f> &camera2d,
      const std::pair<Point2i, Point2i> &viewport);

    Matrix4f m_world_to_screen;
    Matrix4f m_screen_to_world;
  };

  class ZENI_GRAPHICS_DLL Projector3D : public Projector {
  public:
    inline Projector3D(
      const Camera &camera3d = Camera(),
      const std::pair<Point2i, Point2i> &viewport = std::make_pair(Point2i(), get_Video().get_render_target_size()));

    inline Vector3f project(const Vector3f &world_coord) const; ///< Map coordinates in the viewing frustum to screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [0, 1])
    inline Point3f project(const Point3f &world_coord) const; ///< Map coordinates in the viewing frustum to screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [0, 1])

    inline Vector3f unproject(const Vector3f &screen_coord) const; ///< Map screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [0, 1]) to coordinates in the viewing frustum
    inline Point3f unproject(const Point3f &screen_coord) const; ///< Map screen coordinates ([viewport.first.x, viewport.second.x], [viewport.first.y, viewport.second.y], [0, 1]) to coordinates in the viewing frustum

  private:
    inline void init(
      const Camera &camera3d,
      const std::pair<Point2i, Point2i> &viewport);
    
    Matrix4f m_world_to_camera;
    Matrix4f m_camera_to_world;

    Vector3f m_uln;
    Vector3f m_uln2lrf;

    float m_near2far;
  };

}

#endif
