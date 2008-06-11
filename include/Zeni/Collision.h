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

#ifndef ZENI_COLLISION_H
#define ZENI_COLLISION_H

#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>
#include <Zeni/Matrix4f.h>

#include <utility>

namespace Zeni_Collision {

  extern const float g_intersection_epsilon;

  class Sphere;
  class Plane;
  class Line;
  class Ray;
  class Line_Segment;
  class Infinite_Cylinder;
  class Capsule;
  class Parallelepiped;

  class Sphere {
  public:
    Sphere() : center(0.0f, 0.0f, 0.0f), radius(0.0f) {}
    Sphere(const Zeni::Point3f &center_, const float &radius_);
    
    float shortest_distance(const Sphere &rhs) const;
    float shortest_distance(const Zeni::Point3f &rhs) const;
    inline float shortest_distance(const Plane &rhs) const;
    inline float shortest_distance(const Line &rhs) const;
    inline float shortest_distance(const Ray &rhs) const;
    inline float shortest_distance(const Line_Segment &rhs) const;
    inline float shortest_distance(const Infinite_Cylinder &rhs) const;
    inline float shortest_distance(const Capsule &rhs) const;
    inline float shortest_distance(const Parallelepiped &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_center() const {return center;}
    const float & get_radius() const {return radius;}

  private:
    Zeni::Point3f center;
    float radius;
  };

  class Plane {
  public:
    Plane() : point(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 1.0f) {}
    Plane(const Zeni::Point3f &point_, const Zeni::Vector3f &normal_);
    
    float shortest_distance(const Plane &rhs) const;
    float shortest_distance(const Zeni::Point3f &rhs) const;
    float shortest_distance(const Sphere &rhs) const;
    inline float shortest_distance(const Line &rhs) const;
    inline float shortest_distance(const Ray &rhs) const;
    inline float shortest_distance(const Line_Segment &rhs) const;
    inline float shortest_distance(const Infinite_Cylinder &rhs) const;
    inline float shortest_distance(const Capsule &rhs) const;
    inline float shortest_distance(const Parallelepiped &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_point() const {return point;}
    const Zeni::Vector3f & get_normal() const {return normal;}

  private:
    Zeni::Point3f point;
    Zeni::Vector3f normal;
  };

  class Line_Segment {
  public:
    Line_Segment() : end_point_a(0.0f, 0.0f, 0.0f), end_point_b(0.0f, 0.0f, 0.0f) {}
    Line_Segment(const Zeni::Point3f &end_point_a_, const Zeni::Point3f &end_point_b_);
    
    /// Returns <distance, interpolation value [0.0f, 1.0f]>
    std::pair<float, float> nearest_point(const Line_Segment &rhs) const;
    std::pair<float, float> nearest_point(const Zeni::Point3f &rhs) const;
    std::pair<float, float> nearest_point(const Sphere &rhs) const;
    std::pair<float, float> nearest_point(const Plane &rhs) const;
    std::pair<float, float> nearest_point(const Line &rhs) const;
    std::pair<float, float> nearest_point(const Ray &rhs) const;
    std::pair<float, float> nearest_point(const Parallelepiped &rhs) const;

    inline float shortest_distance(const Sphere &rhs) const;
    inline float shortest_distance(const Zeni::Point3f &rhs) const;
    inline float shortest_distance(const Plane &rhs) const;
    inline float shortest_distance(const Line &rhs) const;
    inline float shortest_distance(const Ray &rhs) const;
    inline float shortest_distance(const Line_Segment &rhs) const;
    inline float shortest_distance(const Infinite_Cylinder &rhs) const;
    inline float shortest_distance(const Capsule &rhs) const;
    inline float shortest_distance(const Parallelepiped &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return nearest_point(rhs).first < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_end_point_a() const {return end_point_a;}
    const Zeni::Point3f & get_end_point_b() const {return end_point_b;}
    const Zeni::Vector3f & get_direction() const {return direction;}
    const float & get_direction2() const {return direction2;}
    static bool has_lower_bound() {return true;}
    static bool has_upper_bound() {return true;}

  private:
    Zeni::Point3f end_point_a;
    Zeni::Point3f end_point_b;

    Zeni::Vector3f direction; // end_point_b - end_point_a
    float direction2; // direction * direction
  };

  class Ray {
  public:
    Ray() : end_point_a(0.0f, 0.0f, 0.0f), end_point_b(0.0f, 0.0f, 0.0f) {}
    Ray(const Zeni::Point3f &end_point_a_, const Zeni::Point3f &end_point_b_);
    Ray(const Zeni::Point3f &end_point_a_, const Zeni::Vector3f &direction_vector_);
    
    /// Returns <distance, interpolation value [0.0f, inf)>
    std::pair<float, float> nearest_point(const Ray &rhs) const;
    std::pair<float, float> nearest_point(const Zeni::Point3f &rhs) const;
    std::pair<float, float> nearest_point(const Sphere &rhs) const;
    std::pair<float, float> nearest_point(const Plane &rhs) const;
    std::pair<float, float> nearest_point(const Line_Segment &rhs) const;
    std::pair<float, float> nearest_point(const Line &rhs) const;
    std::pair<float, float> nearest_point(const Parallelepiped &rhs) const;

    inline float shortest_distance(const Sphere &rhs) const;
    inline float shortest_distance(const Zeni::Point3f &rhs) const;
    inline float shortest_distance(const Plane &rhs) const;
    inline float shortest_distance(const Line &rhs) const;
    inline float shortest_distance(const Ray &rhs) const;
    inline float shortest_distance(const Line_Segment &rhs) const;
    inline float shortest_distance(const Infinite_Cylinder &rhs) const;
    inline float shortest_distance(const Capsule &rhs) const;
    inline float shortest_distance(const Parallelepiped &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return nearest_point(rhs).first < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_end_point_a() const {return end_point_a;}
    const Zeni::Point3f & get_end_point_b() const {return end_point_b;}
    const Zeni::Vector3f & get_direction() const {return direction;}
    const float & get_direction2() const {return direction2;}
    static bool has_lower_bound() {return true;}
    static bool has_upper_bound() {return false;}

  private:
    Zeni::Point3f end_point_a;
    Zeni::Point3f end_point_b;

    Zeni::Vector3f direction; // end_point_b - end_point_a
    float direction2; // direction * direction
  };

  class Line {
  public:
    Line() : end_point_a(0.0f, 0.0f, 0.0f), end_point_b(0.0f, 0.0f, 0.0f) {}
    Line(const Zeni::Point3f &end_point_a_, const Zeni::Point3f &end_point_b_);
    Line(const Zeni::Point3f &end_point_a_, const Zeni::Vector3f &direction_vector_);

    float shortest_distance(const Line &rhs) const;
    float shortest_distance(const Zeni::Point3f &rhs) const;
    float shortest_distance(const Sphere &rhs) const;
    float shortest_distance(const Line_Segment &rhs) const;
    float shortest_distance(const Plane &rhs) const;
    float shortest_distance(const Parallelepiped &rhs) const;
    inline float shortest_distance(const Ray &rhs) const;
    inline float shortest_distance(const Infinite_Cylinder &rhs) const;
    inline float shortest_distance(const Capsule &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_end_point_a() const {return end_point_a;}
    const Zeni::Point3f & get_end_point_b() const {return end_point_b;}
    const Zeni::Vector3f & get_direction() const {return direction;}
    const float & get_direction2() const {return direction2;}
    static bool has_lower_bound() {return false;}
    static bool has_upper_bound() {return false;}

  private:
    Zeni::Point3f end_point_a;
    Zeni::Point3f end_point_b;

    Zeni::Vector3f direction; // end_point_b - end_point_a
    float direction2; // direction * direction
  };

  class Infinite_Cylinder {
  public:
    Infinite_Cylinder() : radius(0) {}
    Infinite_Cylinder(const Zeni::Point3f &end_point_a_, const Zeni::Point3f &end_point_b_,
                      const float &radius_);
    Infinite_Cylinder(const Zeni::Point3f &end_point_a_, const Zeni::Vector3f &direction_vector_,
                      const float &radius_);

    float shortest_distance(const Infinite_Cylinder &rhs) const;
    float shortest_distance(const Zeni::Point3f &rhs) const;
    float shortest_distance(const Sphere &rhs) const;
    float shortest_distance(const Plane &rhs) const;
    float shortest_distance(const Line &rhs) const;
    float shortest_distance(const Ray &rhs) const;
    float shortest_distance(const Line_Segment &rhs) const;
    inline float shortest_distance(const Capsule &rhs) const;
    inline float shortest_distance(const Parallelepiped &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_end_point_a() const {return line.get_end_point_a();}
    const Zeni::Point3f & get_end_point_b() const {return line.get_end_point_b();}
    const float & get_radius() const {return radius;}

  private:
    Line line;
    float radius;
  };

  class Capsule {
  public:
    Capsule() : radius(0) {}
    Capsule(const Zeni::Point3f &end_point_a_, const Zeni::Point3f &end_point_b_,
                  const float &radius_);

    /// Returns <distance, interpolation value (-inf, inf)>
    std::pair<float, float> nearest_point(const Capsule &rhs) const;
    std::pair<float, float> nearest_point(const Zeni::Point3f &rhs) const;
    std::pair<float, float> nearest_point(const Sphere &rhs) const;
    std::pair<float, float> nearest_point(const Plane &rhs) const;
    std::pair<float, float> nearest_point(const Line &rhs) const;
    std::pair<float, float> nearest_point(const Ray &rhs) const;
    std::pair<float, float> nearest_point(const Line_Segment &rhs) const;
    std::pair<float, float> nearest_point(const Infinite_Cylinder &rhs) const;
    std::pair<float, float> nearest_point(const Parallelepiped &rhs) const;

    inline float shortest_distance(const Capsule &rhs) const;
    inline float shortest_distance(const Zeni::Point3f &rhs) const;
    inline float shortest_distance(const Sphere &rhs) const;
    inline float shortest_distance(const Plane &rhs) const;
    inline float shortest_distance(const Line &rhs) const;
    inline float shortest_distance(const Ray &rhs) const;
    inline float shortest_distance(const Line_Segment &rhs) const;
    inline float shortest_distance(const Infinite_Cylinder &rhs) const;
    inline float shortest_distance(const Parallelepiped &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return nearest_point(rhs).first < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_end_point_a() const {return line_segment.get_end_point_a();}
    const Zeni::Point3f & get_end_point_b() const {return line_segment.get_end_point_b();}
    const float & get_radius() const {return radius;}

  private:
    Line_Segment line_segment;
    float radius;
  };

  class Parallelepiped {
  public:
    Parallelepiped() : point(0.0f, 0.0f, 0.0f), edge_a(1.0f, 0.0f, 0.0f), edge_b(0.0f, 1.0f, 0.0f), edge_c(0.0f, 0.0f, 1.0f) {}
    Parallelepiped(const Zeni::Point3f &point_, const Zeni::Vector3f &edge_a_,
                   const Zeni::Vector3f &edge_b_, const Zeni::Vector3f &edge_c_);

    float shortest_distance(const Parallelepiped &rhs) const;
    float shortest_distance(const Zeni::Point3f &rhs) const;
    float shortest_distance(const Sphere &rhs) const;
    float shortest_distance(const Plane &rhs) const;
    float shortest_distance(const Infinite_Cylinder &rhs) const;
    inline float shortest_distance(const Line &rhs) const;
    inline float shortest_distance(const Ray &rhs) const;
    inline float shortest_distance(const Line_Segment &rhs) const;
    inline float shortest_distance(const Capsule &rhs) const;

    template <typename TYPE>
    bool intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < g_intersection_epsilon;
    }
    
    const Zeni::Point3f & get_point() const {return point;}
    const Zeni::Vector3f & get_edge_a() const {return edge_a;}
    const Zeni::Vector3f & get_edge_b() const {return edge_b;}
    const Zeni::Vector3f & get_edge_c() const {return edge_c;}
    const Zeni::Matrix4f & get_convert_from() const {return convert_from;}
    const Zeni::Matrix4f & get_convert_to() const {return convert_to;}
    const Zeni::Point3f & get_center() const {return center;}
    const Zeni::Vector3f & get_extents() const {return extents;}
    const Zeni::Vector3f & get_normal_a() const {return normal_a;}
    const Zeni::Vector3f & get_normal_b() const {return normal_b;}
    const Zeni::Vector3f & get_normal_c() const {return normal_c;}

  private:
    Zeni::Point3f point;
    Zeni::Vector3f edge_a;
    Zeni::Vector3f edge_b;
    Zeni::Vector3f edge_c;

    Zeni::Matrix4f convert_from;
    Zeni::Matrix4f convert_to;

    Zeni::Point3f center;
    Zeni::Vector3f extents;
    Zeni::Vector3f normal_a;
    Zeni::Vector3f normal_b;
    Zeni::Vector3f normal_c;
  };

}

#ifdef ZENI_INLINES
#include <Zeni/Collision.hxx>
#endif

#endif
