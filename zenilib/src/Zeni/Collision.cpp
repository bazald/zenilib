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

#include <Zeni/Collision.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>

#include <cmath>

#include <Zeni/Global.h>

namespace Zeni {

  namespace Collision {

    /* Begin Helpers and Templates
     *
     * Hard stuff largely derived from the description here:
     * http://www.softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
     */

    inline std::pair<float, float> unpoof(const std::pair<float, float> &lhs, const float &radii) {
      return std::make_pair(std::max(0.0f, lhs.first - radii), lhs.second);
    }

    inline float unpoof(const float &lhs, const float &radii) {
      return std::max(0.0f, lhs - radii);
    }

    inline void absolute_float_unclamp(float &value, const float &new_value) {
      const float abs_value = fabs(new_value);
      if(value < abs_value)
        value = abs_value;
    }

    template <typename VALUE_TYPE>
    void simple_clamp(VALUE_TYPE &value, const VALUE_TYPE &lower, const VALUE_TYPE &upper) {
      if(value < lower)
        value = lower;
      else if(value > upper)
        value = upper;
    }

    template <typename VALUE_TYPE>
    void double_unclamp(VALUE_TYPE &min_value, VALUE_TYPE &max_value, const VALUE_TYPE &new_value) {
      if(min_value > new_value)
        min_value = new_value;
      else if(max_value < new_value)
        max_value = new_value;
    }

    template <typename LINE_TYPE>
    std::pair<float, float> nearest_point(const LINE_TYPE &lhs, const Point3f &rhs) {
      const Vector3f w  = rhs - lhs.get_end_point_a();
      const Vector3f &u = lhs.get_direction();

      const float &uu = lhs.get_direction2();
      const float uw = u * w;

      Vector3f closest_point = lhs.get_end_point_a() - rhs;

      if(LINE_TYPE::has_lower_bound() && uw < 0.0f)
        return std::make_pair(closest_point.magnitude(), 0.0f);
      else if(LINE_TYPE::has_upper_bound() && uw > uu)
        return std::make_pair((closest_point + u).magnitude(), 1.0f);

      const float t = uw / uu;
      return std::make_pair((closest_point + t * u).magnitude(), t);
    }

    template <typename LINE_TYPE>
    std::pair<float, float> nearest_point(const LINE_TYPE &lhs, const Plane &rhs) {
      const Vector3f w = rhs.get_point() - lhs.get_end_point_a();
      const Vector3f &u = lhs.get_direction();
      const Vector3f &n = rhs.get_normal();

      const float t = (n * w) / (n * u);
      if(LINE_TYPE::has_lower_bound() && t < 0.0f)
        return std::make_pair(rhs.shortest_distance(lhs.get_end_point_a()), 0.0f);
      else if(LINE_TYPE::has_upper_bound() && t > 1.0f)
        return std::make_pair(rhs.shortest_distance(lhs.get_end_point_a() + u), 1.0f);

      return std::make_pair(rhs.shortest_distance(lhs.get_end_point_a() + t * u), t);
    }

    template <typename LINE_TYPE1, typename LINE_TYPE2>
    std::pair<float, float> nearest_point(const LINE_TYPE1 &lhs, const LINE_TYPE2 &rhs) {
      const Vector3f w = lhs.get_end_point_a() - rhs.get_end_point_a();
      const Vector3f &u = lhs.get_direction();
      const Vector3f &v = rhs.get_direction();
      
      const float &uu = lhs.get_direction2();
      const float uv = u * v;
      const float &vv = rhs.get_direction2();
      const float uw = u * w;
      const float vw = v * w;
      
      const float denom = uu * vv - uv * uv;
      float sc_numer = uv * vw - vv * uw, sc_denom;
      float tc_numer = uu * vw - uv * uw, tc_denom;

      if(denom > ZENI_COLLISION_EPSILON) {
        sc_denom = denom;
        
        if(LINE_TYPE1::has_lower_bound() && sc_numer < 0.0f) {
          sc_numer = 0.0f;
          tc_numer = vw;
          tc_denom = vv;
        }
        else if(LINE_TYPE1::has_upper_bound() && sc_numer > sc_denom) {
          sc_numer = sc_denom;
          tc_numer = vw + uv;
          tc_denom = vv;
        }
        else
          tc_denom = denom;
      }
      else {
        sc_numer = 0.0f;
        sc_denom = 1.0f;
        tc_numer = vw;
        tc_denom = vv;
      }
      
      Vector3f min_dist(w);
      float final_numer;
      
      if(LINE_TYPE2::has_lower_bound() && tc_numer < 0.0f)
        final_numer = -uw;
      else if(LINE_TYPE2::has_upper_bound() && tc_numer > tc_denom) {
        min_dist -= v;
        final_numer = uv - uw;
      }
      else {
        const float t = sc_numer / sc_denom;
        return std::make_pair((min_dist + t * u - (tc_numer / tc_denom) * v).magnitude(), t);
      }

      if(LINE_TYPE1::has_lower_bound() && final_numer < 0.0f)
        return std::make_pair(min_dist.magnitude(), 0.0f);
      else if(LINE_TYPE1::has_upper_bound() && final_numer > uu)
        return std::make_pair((min_dist + u).magnitude(), 1.0f);

      const float t = final_numer / uu;
      return std::make_pair((min_dist + t * u).magnitude(), t);
    }

    template <typename LINE_TYPE>
    std::pair<float, float> nearest_point(const LINE_TYPE &lhs, const Parallelepiped &rhs) {
      const Vector3f end_point_a = rhs.get_convert_to() * (lhs.get_end_point_a() - rhs.get_point());
      const Vector3f end_point_b = rhs.get_convert_to() * (lhs.get_end_point_b() - rhs.get_point());
      const Vector3f direction = end_point_b - end_point_a;

      const Vector3f tmin = -end_point_a.divide_by(direction);
      const Vector3f tmax = (Vector3f(1.0f, 1.0f, 1.0f) - end_point_a).divide_by(direction);

      const Vector3f real_min(std::min(tmin.i, tmax.i), std::min(tmin.j, tmax.j), std::min(tmin.k, tmax.k));
      const Vector3f real_max(std::max(tmin.i, tmax.i), std::max(tmin.j, tmax.j), std::max(tmin.k, tmax.k));

      /** Begin Degenerative (Axis Aligned) Safe Code **/

      int valid_axes = 0;
      float invalid_axes_distance2 = 0.0f;
      float min_max = END_OF_TIME;
      float max_min = END_OF_TIME;

      if(fabs(direction.i) > ZENI_COLLISION_EPSILON) {
        if(!valid_axes || real_max.i < min_max) min_max = real_max.i;
        if(!valid_axes || real_min.i > max_min) max_min = real_min.i;
        ++valid_axes;
      }
      else if(end_point_a.i < 0.0f) {
        const Vector3f diff = (rhs.get_convert_from() * Vector3f(end_point_a.i, 0.0f, 0.0f));
        invalid_axes_distance2 += diff * diff;
      }
      else if(end_point_a.i > 1.0f) {
        const Vector3f diff = (rhs.get_convert_from() * Vector3f(end_point_a.i - 1.0f, 0.0f, 0.0f));
        invalid_axes_distance2 += diff * diff;
      }

      if(fabs(direction.j) > ZENI_COLLISION_EPSILON) {
        if(!valid_axes || real_max.j < min_max) min_max = real_max.j;
        if(!valid_axes || real_min.j > max_min) max_min = real_min.j;
        ++valid_axes;
      }
      else if(end_point_a.j < 0.0f) {
        const Vector3f diff = (rhs.get_convert_from() * Vector3f(0.0f, end_point_a.j, 0.0f));
        invalid_axes_distance2 += diff * diff;
      }
      else if(end_point_a.j > 1.0f) {
        const Vector3f diff = (rhs.get_convert_from() * Vector3f(0.0f, end_point_a.j - 1.0f, 0.0f));
        invalid_axes_distance2 += diff * diff;
      }

      if(fabs(direction.k) > ZENI_COLLISION_EPSILON) {
        if(!valid_axes || real_max.k < min_max) min_max = real_max.k;
        if(!valid_axes || real_min.k > max_min) max_min = real_min.k;
        ++valid_axes;
      }
      else if(end_point_a.k < 0.0f) {
        const Vector3f diff = (rhs.get_convert_from() * Vector3f(0.0f, 0.0f, end_point_a.k));
        invalid_axes_distance2 += diff * diff;
      }
      else if(end_point_a.k > 1.0f) {
        const Vector3f diff = (rhs.get_convert_from() * Vector3f(0.0f, 0.0f, end_point_a.k - 1.0f));
        invalid_axes_distance2 += diff * diff;
      }

      if(!valid_axes)
        return std::make_pair(invalid_axes_distance2, 0.0f);

      /** End Degenerative (Axis Aligned) Safe Code **/

      if(LINE_TYPE::has_lower_bound() && min_max < 0.0f)
        min_max = 0.0f;
      else if(LINE_TYPE::has_upper_bound() && min_max > 1.0f)
        min_max = 1.0f;

      if(LINE_TYPE::has_lower_bound() && max_min < 0.0f)
        max_min = 0.0f;
      else if(LINE_TYPE::has_upper_bound() && max_min > 1.0f)
        max_min = 1.0f;

      if(min_max > max_min)
        return std::make_pair(sqrt(invalid_axes_distance2), max_min);

      Point3f closest_point = end_point_a + min_max * direction;
      simple_clamp(closest_point.x, 0.0f, 1.0f);
      simple_clamp(closest_point.y, 0.0f, 1.0f);
      simple_clamp(closest_point.z, 0.0f, 1.0f);
      closest_point = rhs.get_point() + rhs.get_convert_from() * Vector3f(closest_point);

      const Vector3f valid_axes_distance = lhs.get_end_point_a() + min_max * lhs.get_direction() - closest_point;
      const float valid_axes_distance2 = valid_axes_distance * valid_axes_distance;
      const float total_distance = sqrt(invalid_axes_distance2 + valid_axes_distance2);

      return std::make_pair(total_distance, min_max);
    }

    /* End Helpers and Templates
     */

    Sphere::Sphere(const Point3f &center_, const float &radius_)
      : center(center_),
      radius(radius_)
    {
    }

    float Sphere::shortest_distance(const Sphere &rhs) const {
      return unpoof((center - rhs.center).magnitude(), radius + rhs.radius);
    }
    float Sphere::shortest_distance(const Point3f &rhs) const {
      return unpoof((center - rhs).magnitude(), radius);
    }

    Plane::Plane(const Point3f &point_, const Vector3f &normal_)
      : point(point_),
      normal(normal_.normalized())
    {
    }

    float Plane::shortest_distance(const Plane &rhs) const {
      const Vector3f line_normal = normal % rhs.normal;
      if(line_normal.magnitude() < ZENI_COLLISION_EPSILON)
        return shortest_distance(rhs.point);

      return 0.0f;
    }

    float Plane::shortest_distance(const Point3f &rhs) const {
      return fabs((point - rhs) * normal);
    }

    float Plane::shortest_distance(const Sphere &rhs) const {
      return unpoof(shortest_distance(rhs.get_center()), rhs.get_radius());
    }

    Line::Line(const Point3f &end_point_a_, const Point3f &end_point_b_)
      : end_point_a(end_point_a_),
      end_point_b(end_point_b_),
      direction(end_point_b - end_point_a),
      direction2(direction * direction)
    {
    }

    Line::Line(const Point3f &end_point_a_, const Vector3f &direction_vector_)
      : end_point_a(end_point_a_),
      end_point_b(end_point_a_ + direction_vector_),
      direction(direction_vector_),
      direction2(direction * direction)
    {
    }

    float Line::shortest_distance(const Point3f &rhs) const {
      return Collision::nearest_point(*this, rhs).first;
    }
    float Line::shortest_distance(const Sphere &rhs) const {
      return unpoof(shortest_distance(rhs.get_center()), rhs.get_radius());
    }
    float Line::shortest_distance(const Plane &rhs) const {
      return Collision::nearest_point(*this, rhs).first;
    }
    float Line::shortest_distance(const Line &rhs) const {
      return Collision::nearest_point(*this, rhs).first;
    }
    float Line::shortest_distance(const Line_Segment &rhs) const {
      return Collision::nearest_point(*this, rhs).first;
    }
    float Line::shortest_distance(const Parallelepiped &rhs) const {
      return Collision::nearest_point(*this, rhs).first;
    }

    Ray::Ray(const Point3f &end_point_a_, const Point3f &end_point_b_)
      : end_point_a(end_point_a_),
      end_point_b(end_point_b_),
      direction(end_point_b - end_point_a),
      direction2(direction * direction)
    {
    }

    Ray::Ray(const Point3f &end_point_a_, const Vector3f &direction_vector_)
      : end_point_a(end_point_a_),
      end_point_b(end_point_a_ + direction_vector_),
      direction(direction_vector_),
      direction2(direction * direction)
    {
    }

    std::pair<float, float> Ray::nearest_point(const Point3f &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Ray::nearest_point(const Sphere &rhs) const {
      return unpoof(nearest_point(rhs.get_center()), rhs.get_radius());
    }
    std::pair<float, float> Ray::nearest_point(const Plane &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Ray::nearest_point(const Line &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Ray::nearest_point(const Ray &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Ray::nearest_point(const Line_Segment &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Ray::nearest_point(const Parallelepiped &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }

    Line_Segment::Line_Segment(const Point3f &end_point_a_, const Point3f &end_point_b_)
      : end_point_a(end_point_a_),
      end_point_b(end_point_b_),
      direction(end_point_b - end_point_a),
      direction2(direction * direction)
    {
    }

    std::pair<float, float> Line_Segment::nearest_point(const Point3f &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Line_Segment::nearest_point(const Sphere &rhs) const {
      return unpoof(nearest_point(rhs.get_center()), rhs.get_radius());
    }
    std::pair<float, float> Line_Segment::nearest_point(const Plane &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Line_Segment::nearest_point(const Line &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Line_Segment::nearest_point(const Ray &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Line_Segment::nearest_point(const Line_Segment &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }
    std::pair<float, float> Line_Segment::nearest_point(const Parallelepiped &rhs) const {
      return Collision::nearest_point(*this, rhs);
    }

    Infinite_Cylinder::Infinite_Cylinder(const Point3f &end_point_a_, const Point3f &end_point_b_, 
                                 const float &radius_)
      : line(end_point_a_, end_point_b_),
      radius(radius_)
    {
    }

    Infinite_Cylinder::Infinite_Cylinder(const Point3f &end_point_a_, const Vector3f &direction_vector_, 
                                 const float &radius_)
      : line(end_point_a_, direction_vector_),
      radius(radius_)
    {
    }

    float Infinite_Cylinder::shortest_distance(const Point3f &rhs) const {
      return unpoof(line.shortest_distance(rhs), radius);
    }
    float Infinite_Cylinder::shortest_distance(const Sphere &rhs) const {
      return unpoof(line.shortest_distance(rhs), radius);
    }
    float Infinite_Cylinder::shortest_distance(const Plane &rhs) const {
      return unpoof(line.shortest_distance(rhs), radius);
    }
    float Infinite_Cylinder::shortest_distance(const Infinite_Cylinder &rhs) const {
      return unpoof(line.shortest_distance(rhs.line), radius + rhs.radius);
    }
    float Infinite_Cylinder::shortest_distance(const Line &rhs) const {
      return unpoof(line.shortest_distance(rhs), radius);
    }
    float Infinite_Cylinder::shortest_distance(const Ray &rhs) const {
      return unpoof(line.shortest_distance(rhs), radius);
    }
    float Infinite_Cylinder::shortest_distance(const Line_Segment &rhs) const {
      return unpoof(line.shortest_distance(rhs), radius);
    }

    Capsule::Capsule(const Point3f &end_point_a_, const Point3f &end_point_b_, 
                                 const float &radius_)
      : line_segment(end_point_a_, end_point_b_),
      radius(radius_)
    {
    }

    std::pair<float, float> Capsule::nearest_point(const Point3f &rhs) const {
      return unpoof(line_segment.nearest_point(rhs), radius);
    }
    std::pair<float, float> Capsule::nearest_point(const Sphere &rhs) const {
      return unpoof(line_segment.nearest_point(rhs), radius);
    }
    std::pair<float, float> Capsule::nearest_point(const Plane &rhs) const {
      return unpoof(line_segment.nearest_point(rhs), radius);
    }
    std::pair<float, float> Capsule::nearest_point(const Line &rhs) const {
      return unpoof(line_segment.nearest_point(rhs), radius);
    }
    std::pair<float, float> Capsule::nearest_point(const Ray &rhs) const {
      return unpoof(line_segment.nearest_point(rhs), radius);
    }
    std::pair<float, float> Capsule::nearest_point(const Line_Segment &rhs) const {
      return unpoof(line_segment.nearest_point(rhs), radius);
    }
    std::pair<float, float> Capsule::nearest_point(const Infinite_Cylinder &rhs) const {
      return unpoof(line_segment.nearest_point(reinterpret_cast<const Line_Segment &>(rhs)), radius + rhs.get_radius());
    }
    std::pair<float, float> Capsule::nearest_point(const Capsule &rhs) const {
      return unpoof(line_segment.nearest_point(rhs.line_segment), radius + rhs.radius);
    }
    std::pair<float, float> Capsule::nearest_point(const Parallelepiped &rhs) const {
      return unpoof(line_segment.nearest_point(rhs), radius);
    }
    
    Parallelepiped::Parallelepiped(const Point3f &point_, const Vector3f &edge_a_,
                                   const Vector3f &edge_b_, const Vector3f &edge_c_)
      : point(point_),
      edge_a(edge_a_),
      edge_b(edge_b_),
      edge_c(edge_c_),
      convert_from(edge_a, edge_b, edge_c),
      convert_to(convert_from.inverted()),
      center(point),
      normal_a((edge_b % edge_c).normalized()),
      normal_b((edge_c % edge_a).normalized()),
      normal_c((edge_a % edge_b).normalized())
    {
      const Vector3f ha = 0.5f * edge_a;
      const Vector3f hb = 0.5f * edge_b;
      const Vector3f hc = 0.5f * edge_c;

      center += ha + hb + hc;

      absolute_float_unclamp(extents.i, ha * normal_a);
      absolute_float_unclamp(extents.i, hb * normal_a);
      absolute_float_unclamp(extents.i, hc * normal_a);
      absolute_float_unclamp(extents.j, ha * normal_b);
      absolute_float_unclamp(extents.j, hb * normal_b);
      absolute_float_unclamp(extents.j, hc * normal_b);
      absolute_float_unclamp(extents.k, ha * normal_c);
      absolute_float_unclamp(extents.k, hb * normal_c);
      absolute_float_unclamp(extents.k, hc * normal_c);
    }

    float Parallelepiped::shortest_distance(const Parallelepiped &rhs) const {
      const Vector3f &a = extents;
      const Point3f &Pa = center;
      const Vector3f * const A = &normal_a;

      const Vector3f &b = rhs.extents;
      const Point3f &Pb = rhs.center;
      const Vector3f * const B = &rhs.normal_a;

      // translation, in parent frame
      const Vector3f v = Pb - Pa;

      // translation, in A's frame
      const Vector3f T(v * A[0], v * A[1], v * A[2]);

      // B's basis with respect to A's local frame (rotation matrix)
      float R[3][3];
      for(int j = 0; j < 3; ++j)
        for(int i = 0; i < 3; ++i)
          R[j][i] = A[j] * B[i]; 

      // generate the fabs(rotation matrix)
      const float abs_R[3][3] = {{float(fabs(R[0][0])), float(fabs(R[0][1])), float(fabs(R[0][2]))},
                                 {float(fabs(R[1][0])), float(fabs(R[1][1])), float(fabs(R[1][2]))},
                                 {float(fabs(R[2][0])), float(fabs(R[2][1])), float(fabs(R[2][2]))}};

      /* ALGORITHM: Use the separating axis test for all 15 potential
       * separating axes. If a separating axis could not be found, the two
       * boxes overlap.
       */

      // A and B's 6 basis vectors (3 each)
      for(int i = 0; i < 3; ++i) {
        {
          const float &ra = a[i];
          const float rb = b[0] * abs_R[i][0] + b[1] * abs_R[i][1] + b[2] * abs_R[i][2];
          const float t = fabs(T[i]);

          if(t > ra + rb)
            return t - (ra + rb);
        }

        {
          const float ra = a[0] * abs_R[0][i] + a[1] * abs_R[1][i] + a[2] * abs_R[2][i];
          const float &rb = b[i];
          const float t = fabs(T[0] * R[0][i] + T[1] * R[1][i] + T[2] * R[2][i]);

          if(t > ra + rb)
            return t - (ra + rb);
        }
      }

      /* 9 cross products
       *
       * L = A/j[0,1,2] x B/i[0,1,2]
       *
       * row by row  | column by column
       * j = [0,1,2] | i = [0,1,2]
       * u = [1,0,0] | m = [1,0,0]
       * v = [2,2,1] | n = [2,2,1]
       */
      for(int j = 0, u = 1, v = 2; j < 3; ++j) {
        for(int i = 0, m = 1, n = 2; i < 3; ++i) {
          const float ra = a[u] * abs_R[v][i] + a[v] * abs_R[u][i];
          const float rb = b[m] * abs_R[j][n] + b[n] * abs_R[j][m];
          const float t = fabs(T[u] * R[v][i] - T[v] * R[u][i]);

          if(t > ra + rb)
            return t - (ra + rb);

          if(!i) --m; else --n;
        }

        if(!j) --u; else --v;
      }

      // no separating axis found, indicating that the two boxes overlap
      return 0.0f;
    }

    float Parallelepiped::shortest_distance(const Point3f &rhs) const {
      const Point3f converted_point = convert_to * (rhs - point);

      Point3f nearest_point = converted_point;
      simple_clamp(nearest_point.x, 0.0f, 1.0f);
      simple_clamp(nearest_point.y, 0.0f, 1.0f);
      simple_clamp(nearest_point.z, 0.0f, 1.0f);

      const Vector3f difference = convert_from * (converted_point - nearest_point);

      return difference.magnitude();
    }

    float Parallelepiped::shortest_distance(const Plane &rhs) const {
      const Point3f &p = rhs.get_point();
      const Vector3f &n = rhs.get_normal();

      const float diff_001 = edge_c * n;
      const float diff_010 = edge_b * n;
      const float diff_011 = diff_010 + diff_001;
      const float diff_100 = edge_a * n;
      const float diff_101 = diff_100 + diff_001;
      const float diff_110 = diff_100 + diff_010;
      const float diff_111 = diff_100 + diff_011;

      const float t_000 = (point - p) * n;
      const float t_001 = t_000 + diff_001;
      const float t_010 = t_000 + diff_010;
      const float t_011 = t_000 + diff_011;
      const float t_100 = t_000 + diff_100;
      const float t_101 = t_000 + diff_101;
      const float t_110 = t_000 + diff_110;
      const float t_111 = t_000 + diff_111;

      float min_t = t_000;
      float max_t = t_000;

      double_unclamp(min_t, max_t, t_001);
      double_unclamp(min_t, max_t, t_010);
      double_unclamp(min_t, max_t, t_011);
      double_unclamp(min_t, max_t, t_100);
      double_unclamp(min_t, max_t, t_101);
      double_unclamp(min_t, max_t, t_110);
      double_unclamp(min_t, max_t, t_111);

      if(min_t > 0.0f)
        return min_t;
      if(max_t < 0.0f)
        return -max_t;
      return 0.0f;
    }

    float Parallelepiped::shortest_distance(const Sphere &rhs) const {
      return unpoof(shortest_distance(rhs.get_center()), rhs.get_radius());
    }
    float Parallelepiped::shortest_distance(const Infinite_Cylinder &rhs) const {
      return unpoof(shortest_distance(reinterpret_cast<const Line &>(rhs)), rhs.get_radius());
    }

  }

}

#include <Zeni/Global_Undef.h>
