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

#ifndef ZENI_PROJECTOR_HXX
#define ZENI_PROJECTOR_HXX

// HXXed below
#include <Zeni/Camera.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Video.h>

#include <Zeni/Projector.h>

namespace Zeni {

  Projector::Projector(const std::pair<Point2i, Point2i> &viewport)
  {
    init(viewport);
  }

  Projector::~Projector()
  {
  }

  void Projector::init(const std::pair<Point2i, Point2i> &viewport)
  {
    m_offset = Vector3f(float(viewport.first.x), float(viewport.first.y), 0.0f);
    m_size = Vector3f(float(viewport.second.x - viewport.first.x), float(viewport.second.y - viewport.first.y), 1.0f);
  }

  const Vector3f & Projector::offset() const {
    return m_offset;
  }

  const Vector3f & Projector::size() const {
    return m_size;
  }
  
  Projector2D::Projector2D(
    const std::pair<Point2f, Point2f> &camera2d,
    const std::pair<Point2i, Point2i> &viewport)
  {
    init(camera2d, viewport);
  }

  Vector3f Projector2D::project(const Vector3f &world_coord) const {
    return m_world_to_screen * world_coord;
  }

  Point3f Projector2D::project(const Point3f &world_coord) const {
    return project(Vector3f(world_coord));
  }

  Point2f Projector2D::project(const Point2f &world_coord) const {
    return Point2f(project(Vector3f(Point3f(world_coord))));
  }

  Vector3f Projector2D::unproject(const Vector3f &screen_coord) const {
    return m_screen_to_world * screen_coord;
  }

  Point3f Projector2D::unproject(const Point3f &screen_coord) const {
    return unproject(Vector3f(screen_coord));
  }

  Point2f Projector2D::unproject(const Point2f &screen_coord) const {
    return Point2f(unproject(Vector3f(Point3f(screen_coord))));
  }

  void Projector2D::init(
    const std::pair<Point2f, Point2f> &camera2d,
    const std::pair<Point2i, Point2i> &viewport)
  {
    Projector::init(viewport);

    m_world_to_screen =
      Matrix4f::Orthographic(float(viewport.first.x), float(viewport.second.x), float(viewport.second.y), float(viewport.first.y), -1.0f, 1.0f).inverted() *
      Matrix4f::Orthographic(camera2d.first.x, camera2d.second.x, camera2d.second.y, camera2d.first.y, -1.0f, 1.0f);

    m_screen_to_world = m_world_to_screen.inverted();
  }

  Projector3D::Projector3D(
    const Camera &camera3d,
    const std::pair<Point2i, Point2i> &viewport)
  {
    init(camera3d, viewport);
  }

  Vector3f Projector3D::project(const Vector3f &world_coord) const {
    const float z_value = (Vector3f(m_world_to_camera * world_coord - m_uln).divide_by(m_uln2lrf)).k;
    const float scale = z_value * m_near2far + 1.0f - z_value;
    return (((m_world_to_camera * world_coord).divide_by(Vector3f(scale, scale, 1.0f)) - m_uln).divide_by(m_uln2lrf)).multiply_by(size()) + offset();
  }

  Point3f Projector3D::project(const Point3f &world_coord) const {
    return project(Vector3f(world_coord));
  }

  Vector3f Projector3D::unproject(const Vector3f &screen_coord) const {
    const float scale = screen_coord.k * m_near2far + 1.0f - screen_coord.k;
    return m_camera_to_world * ((screen_coord - offset()).divide_by(size()).multiply_by(m_uln2lrf) + m_uln).multiply_by(Vector3f(scale, scale, 1.0f));
  }

  Point3f Projector3D::unproject(const Point3f &screen_coord) const {
    return unproject(Vector3f(screen_coord));
  }

  void Projector3D::init(
    const Camera &camera3d,
    const std::pair<Point2i, Point2i> &viewport)
  {
    Projector::init(viewport);

    m_world_to_camera = camera3d.get_view_matrix();
    m_camera_to_world = m_world_to_camera.inverted();

    const float aspect = size().i / size().j;
    const float top = float(tan(0.5f * camera3d.fov_rad)) * camera3d.near_clip;
    const float bottom = -top;
    const float left = aspect * bottom;
    const float right = aspect * top;

    const float tunneled_near_clip = camera3d.get_tunneled_near_clip();
    const float tunneled_far_clip = camera3d.get_tunneled_far_clip();

    m_uln = Vector3f(left, top, -tunneled_near_clip);
    m_uln2lrf = Vector3f(right, bottom, -tunneled_far_clip) - m_uln;

    m_near2far = tunneled_far_clip / tunneled_near_clip;
  }

}

#include <Zeni/Camera.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Video.hxx>

#endif
