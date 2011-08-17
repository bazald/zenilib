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
 * \class Zeni::Collision::Sphere
 *
 * \ingroup zenilib
 *
 * \brief Collision Sphere
 *
 * This class ZENI_DLL describes an Sphere object in 3-space.  The  
 * Sphere extends the Point3f to have a radius.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Collision::Plane
 *
 * \ingroup zenilib
 *
 * \brief Collision Plane
 *
 * This class ZENI_DLL describes a Plane object in 3-space.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Collision::Line_Segment
 *
 * \ingroup zenilib
 *
 * \brief Collision Line Segment
 *
 * This class ZENI_DLL describes a Line Segment object in 3-space.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.  Additionally, it is possible to determine the closest point on
 * the Line_Segment to the object it is being compared to.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Collision::Ray
 *
 * \ingroup zenilib
 *
 * \brief Collision Ray
 *
 * This class ZENI_DLL describes a Ray object in 3-space.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.  Additionally, it is possible to determine the closest point on
 * the Ray to the object it is being compared to.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Collision::Line
 *
 * \ingroup zenilib
 *
 * \brief Collision Line
 *
 * This class ZENI_DLL describes a Line object in 3-space.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Collision::Infinite_Cylinder
 *
 * \ingroup zenilib
 *
 * \brief Collision Infinite Cylinder
 *
 * This class ZENI_DLL describes an Infinite Cylinder object in 3-space.  The  
 * Infinite_Cylinder extends the Line to have a radius.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Collision::Capsule
 *
 * \ingroup zenilib
 *
 * \brief Collision Capsule
 *
 * This class ZENI_DLL describes a Capsule object in 3-space.  The Capsule 
 * extends the Line_Segment to have a radius.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.  Additionally, it is possible to determine the closest point on
 * the Capsule's inner Line Segment to the object it is being compared to.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Collision::Parallelepiped
 *
 * \ingroup zenilib
 *
 * \brief Collision Parallelepiped
 *
 * This class ZENI_DLL describes a Parallelepiped object in 3-space.  It is possible to test 
 * to see if it intersects any other object in Zeni_Collision, and it is also 
 * possible to find the shortest distance between it and any other object in 
 * Zeni_Collision.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_COLLISION_H
#define ZENI_COLLISION_H

#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>
#include <Zeni/Matrix4f.h>

#include <utility>

namespace Zeni {
  namespace Collision {

    class ZENI_DLL Sphere;
    class ZENI_DLL Plane;
    class ZENI_DLL Line;
    class ZENI_DLL Ray;
    class ZENI_DLL Line_Segment;
    class ZENI_DLL Infinite_Cylinder;
    class ZENI_DLL Capsule;
    class ZENI_DLL Parallelepiped;

    class ZENI_DLL Sphere {
    public:
      Sphere() : center(0.0f, 0.0f, 0.0f), radius(0.0f) {}
      Sphere(const Point3f &center_, const float &radius_);
      
      float shortest_distance(const Sphere &rhs) const;
      float shortest_distance(const Point3f &rhs) const;
      inline float shortest_distance(const Plane &rhs) const;
      inline float shortest_distance(const Line &rhs) const;
      inline float shortest_distance(const Ray &rhs) const;
      inline float shortest_distance(const Line_Segment &rhs) const;
      inline float shortest_distance(const Infinite_Cylinder &rhs) const;
      inline float shortest_distance(const Capsule &rhs) const;
      inline float shortest_distance(const Parallelepiped &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_center() const {return center;}
      const float & get_radius() const {return radius;}

    private:
      Point3f center;
      float radius;
    };

    class ZENI_DLL Plane {
    public:
      Plane() : point(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 1.0f) {}
      Plane(const Point3f &point_, const Vector3f &normal_);
      
      float shortest_distance(const Plane &rhs) const;
      float shortest_distance(const Point3f &rhs) const;
      float shortest_distance(const Sphere &rhs) const;
      inline float shortest_distance(const Line &rhs) const;
      inline float shortest_distance(const Ray &rhs) const;
      inline float shortest_distance(const Line_Segment &rhs) const;
      inline float shortest_distance(const Infinite_Cylinder &rhs) const;
      inline float shortest_distance(const Capsule &rhs) const;
      inline float shortest_distance(const Parallelepiped &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_point() const {return point;}
      const Vector3f & get_normal() const {return normal;}

    private:
      Point3f point;
      Vector3f normal;
    };

    class ZENI_DLL Line_Segment {
    public:
      Line_Segment() : end_point_a(0.0f, 0.0f, 0.0f), end_point_b(0.0f, 0.0f, 0.0f) {}
      Line_Segment(const Point3f &end_point_a_, const Point3f &end_point_b_);
      
      /// Returns <distance, interpolation value [0.0f, 1.0f]>
      std::pair<float, float> nearest_point(const Line_Segment &rhs) const;
      std::pair<float, float> nearest_point(const Point3f &rhs) const;
      std::pair<float, float> nearest_point(const Sphere &rhs) const;
      std::pair<float, float> nearest_point(const Plane &rhs) const;
      std::pair<float, float> nearest_point(const Line &rhs) const;
      std::pair<float, float> nearest_point(const Ray &rhs) const;
      std::pair<float, float> nearest_point(const Parallelepiped &rhs) const;

      inline float shortest_distance(const Sphere &rhs) const;
      inline float shortest_distance(const Point3f &rhs) const;
      inline float shortest_distance(const Plane &rhs) const;
      inline float shortest_distance(const Line &rhs) const;
      inline float shortest_distance(const Ray &rhs) const;
      inline float shortest_distance(const Line_Segment &rhs) const;
      inline float shortest_distance(const Infinite_Cylinder &rhs) const;
      inline float shortest_distance(const Capsule &rhs) const;
      inline float shortest_distance(const Parallelepiped &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_end_point_a() const {return end_point_a;}
      const Point3f & get_end_point_b() const {return end_point_b;}
      const Vector3f & get_direction() const {return direction;}
      const float & get_direction2() const {return direction2;}
      static bool has_lower_bound() {return true;}
      static bool has_upper_bound() {return true;}

    private:
      Point3f end_point_a;
      Point3f end_point_b;

      Vector3f direction; // end_point_b - end_point_a
      float direction2; // direction * direction
    };

    class ZENI_DLL Ray {
    public:
      Ray() : end_point_a(0.0f, 0.0f, 0.0f), end_point_b(0.0f, 0.0f, 0.0f) {}
      Ray(const Point3f &end_point_a_, const Point3f &end_point_b_);
      Ray(const Point3f &end_point_a_, const Vector3f &direction_vector_);
      
      /// Returns <distance, interpolation value [0.0f, inf)>
      std::pair<float, float> nearest_point(const Ray &rhs) const;
      std::pair<float, float> nearest_point(const Point3f &rhs) const;
      std::pair<float, float> nearest_point(const Sphere &rhs) const;
      std::pair<float, float> nearest_point(const Plane &rhs) const;
      std::pair<float, float> nearest_point(const Line_Segment &rhs) const;
      std::pair<float, float> nearest_point(const Line &rhs) const;
      std::pair<float, float> nearest_point(const Parallelepiped &rhs) const;

      inline float shortest_distance(const Sphere &rhs) const;
      inline float shortest_distance(const Point3f &rhs) const;
      inline float shortest_distance(const Plane &rhs) const;
      inline float shortest_distance(const Line &rhs) const;
      inline float shortest_distance(const Ray &rhs) const;
      inline float shortest_distance(const Line_Segment &rhs) const;
      inline float shortest_distance(const Infinite_Cylinder &rhs) const;
      inline float shortest_distance(const Capsule &rhs) const;
      inline float shortest_distance(const Parallelepiped &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_end_point_a() const {return end_point_a;}
      const Point3f & get_end_point_b() const {return end_point_b;}
      const Vector3f & get_direction() const {return direction;}
      const float & get_direction2() const {return direction2;}
      static bool has_lower_bound() {return true;}
      static bool has_upper_bound() {return false;}

    private:
      Point3f end_point_a;
      Point3f end_point_b;

      Vector3f direction; // end_point_b - end_point_a
      float direction2; // direction * direction
    };

    class ZENI_DLL Line {
    public:
      Line() : end_point_a(0.0f, 0.0f, 0.0f), end_point_b(0.0f, 0.0f, 0.0f) {}
      Line(const Point3f &end_point_a_, const Point3f &end_point_b_);
      Line(const Point3f &end_point_a_, const Vector3f &direction_vector_);

      float shortest_distance(const Line &rhs) const;
      float shortest_distance(const Point3f &rhs) const;
      float shortest_distance(const Sphere &rhs) const;
      float shortest_distance(const Line_Segment &rhs) const;
      float shortest_distance(const Plane &rhs) const;
      float shortest_distance(const Parallelepiped &rhs) const;
      inline float shortest_distance(const Ray &rhs) const;
      inline float shortest_distance(const Infinite_Cylinder &rhs) const;
      inline float shortest_distance(const Capsule &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_end_point_a() const {return end_point_a;}
      const Point3f & get_end_point_b() const {return end_point_b;}
      const Vector3f & get_direction() const {return direction;}
      const float & get_direction2() const {return direction2;}
      static bool has_lower_bound() {return false;}
      static bool has_upper_bound() {return false;}

    private:
      Point3f end_point_a;
      Point3f end_point_b;

      Vector3f direction; // end_point_b - end_point_a
      float direction2; // direction * direction
    };

    class ZENI_DLL Infinite_Cylinder {
    public:
      Infinite_Cylinder() : radius(0) {}
      Infinite_Cylinder(const Point3f &end_point_a_, const Point3f &end_point_b_,
                                              const float &radius_);
      Infinite_Cylinder(const Point3f &end_point_a_, const Vector3f &direction_vector_,
                                              const float &radius_);

      float shortest_distance(const Infinite_Cylinder &rhs) const;
      float shortest_distance(const Point3f &rhs) const;
      float shortest_distance(const Sphere &rhs) const;
      float shortest_distance(const Plane &rhs) const;
      float shortest_distance(const Line &rhs) const;
      float shortest_distance(const Ray &rhs) const;
      float shortest_distance(const Line_Segment &rhs) const;
      inline float shortest_distance(const Capsule &rhs) const;
      inline float shortest_distance(const Parallelepiped &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_end_point_a() const {return line.get_end_point_a();}
      const Point3f & get_end_point_b() const {return line.get_end_point_b();}
      const float & get_radius() const {return radius;}

    private:
      Line line;
      float radius;
    };

    class ZENI_DLL Capsule {
    public:
      Capsule() : radius(0) {}
      Capsule(const Point3f &end_point_a_, const Point3f &end_point_b_,
                                    const float &radius_);

      /// Returns <distance, interpolation value [0.0f, 1.0f]>
      std::pair<float, float> nearest_point(const Capsule &rhs) const;
      std::pair<float, float> nearest_point(const Point3f &rhs) const;
      std::pair<float, float> nearest_point(const Sphere &rhs) const;
      std::pair<float, float> nearest_point(const Plane &rhs) const;
      std::pair<float, float> nearest_point(const Line &rhs) const;
      std::pair<float, float> nearest_point(const Ray &rhs) const;
      std::pair<float, float> nearest_point(const Line_Segment &rhs) const;
      std::pair<float, float> nearest_point(const Infinite_Cylinder &rhs) const;
      std::pair<float, float> nearest_point(const Parallelepiped &rhs) const;

      inline float shortest_distance(const Capsule &rhs) const;
      inline float shortest_distance(const Point3f &rhs) const;
      inline float shortest_distance(const Sphere &rhs) const;
      inline float shortest_distance(const Plane &rhs) const;
      inline float shortest_distance(const Line &rhs) const;
      inline float shortest_distance(const Ray &rhs) const;
      inline float shortest_distance(const Line_Segment &rhs) const;
      inline float shortest_distance(const Infinite_Cylinder &rhs) const;
      inline float shortest_distance(const Parallelepiped &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_end_point_a() const {return line_segment.get_end_point_a();}
      const Point3f & get_end_point_b() const {return line_segment.get_end_point_b();}
      const float & get_radius() const {return radius;}

    private:
      Line_Segment line_segment;
      float radius;
    };

    class ZENI_DLL Parallelepiped {
    public:
      Parallelepiped() : point(0.0f, 0.0f, 0.0f), edge_a(1.0f, 0.0f, 0.0f), edge_b(0.0f, 1.0f, 0.0f), edge_c(0.0f, 0.0f, 1.0f) {}
      Parallelepiped(const Point3f &point_, const Vector3f &edge_a_,
                                           const Vector3f &edge_b_, const Vector3f &edge_c_);

      float shortest_distance(const Parallelepiped &rhs) const;
      float shortest_distance(const Point3f &rhs) const;
      float shortest_distance(const Sphere &rhs) const;
      float shortest_distance(const Plane &rhs) const;
      float shortest_distance(const Infinite_Cylinder &rhs) const;
      inline float shortest_distance(const Line &rhs) const;
      inline float shortest_distance(const Ray &rhs) const;
      inline float shortest_distance(const Line_Segment &rhs) const;
      inline float shortest_distance(const Capsule &rhs) const;

      template <typename TYPE>
      bool intersects(const TYPE &rhs) const;
      
      const Point3f & get_point() const {return point;}
      const Vector3f & get_edge_a() const {return edge_a;}
      const Vector3f & get_edge_b() const {return edge_b;}
      const Vector3f & get_edge_c() const {return edge_c;}
      const Matrix4f & get_convert_from() const {return convert_from;}
      const Matrix4f & get_convert_to() const {return convert_to;}
      const Point3f & get_center() const {return center;}
      const Vector3f & get_extents() const {return extents;}
      const Vector3f & get_normal_a() const {return normal_a;}
      const Vector3f & get_normal_b() const {return normal_b;}
      const Vector3f & get_normal_c() const {return normal_c;}

    private:
      Point3f point;
      Vector3f edge_a;
      Vector3f edge_b;
      Vector3f edge_c;

      Matrix4f convert_from;
      Matrix4f convert_to;

      Point3f center;
      Vector3f extents;
      Vector3f normal_a;
      Vector3f normal_b;
      Vector3f normal_c;
    };

  }

}

#endif
