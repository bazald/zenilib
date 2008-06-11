/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include "zenilib.h"

#include <Zeni/Fonts.h>
#include <Zeni/Sounds.h>
#include <Zeni/Textures.h>

#include <Zeni/Sound.hxx>
#include <Zeni/Gamestate_One.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace Zeni;
using namespace Zeni_Collision;
using Zeni_Collision::Line_Segment;

static vector<string> g_args;

typedef Zeni_Collision::Plane Collider_Type;
const float segment_length = 10.0f;

class MyCustomGamestate : public Gamestate_Base {
public:
  MyCustomGamestate();

private:
  void perform_logic();
  void render();

  void render_sphere(const Sphere &sphere, const bool &red);

  Time m_time;

  Camera m_camera;

  float m_angle;
  Vector3f m_a, m_b;
  Collider_Type m_collider;
  Parallelepiped m_parallelepiped;

  Model m_blue_sphere, m_red_sphere;
  Model m_blue_cube, m_red_cube;

  typedef vector<pair<Sphere, bool> > Spheres;
  Spheres m_spheres;

  typedef vector<pair<Zeni_Collision::Line_Segment, bool> > Line_Segments;
  Line_Segments m_line_segments;

  typedef vector<pair<Parallelepiped, bool> > Parallelepipeds;
  Parallelepipeds m_parallelepipeds;
};

MyCustomGamestate::MyCustomGamestate()
: m_time(Timer::get_reference().get_time()),
m_angle(0.0f),
m_blue_sphere("models/blue_sphere.3DS"),
m_red_sphere("models/red_sphere.3DS"),
m_blue_cube("models/blue_cube.3DS"),
m_red_cube("models/red_cube.3DS")
{
  m_camera.set_position(Point3f(0.0f, 0.0f, 100.0f));
  m_camera.set_forward(Vector3f(0.0f, 0.0f, -1.0f));
  m_camera.set_up(Vector3f(0.0f, 1.0f, 0.0f));
  m_camera.set_fov_rad(pi_over_two);

  m_spheres.push_back(make_pair(Sphere(Point3f(-20.0f, 20.0f, 0.0f), 5.0f), false));
  m_spheres.push_back(make_pair(Sphere(Point3f(-25.0f, -25.0f, 0.0f), 10.0f), false));
  m_spheres.push_back(make_pair(Sphere(Point3f(30.0f, -30.0f, 0.0f), 15.0f), false));
  //m_spheres.push_back(make_pair(Sphere(Point3f(35.0f, 35.0f, 0.0f), 20.0f), false));

  m_line_segments.push_back(make_pair(Zeni_Collision::Line_Segment(Point3f(-3.0f, 50.0f, 0.0f), Point3f(3.0f, 10.0f, 0.0f)), false));
  m_line_segments.push_back(make_pair(Zeni_Collision::Line_Segment(Point3f(-50.0f, -20.0f, 0.0f), Point3f(-50.0f, 20.0f, 0.0f)), false));

  m_parallelepipeds.push_back(make_pair(Parallelepiped(Point3f(15.0f, 15.0f, -20.0f), 40.0f * vector_i, 40.0f * vector_j, 40.0f * vector_k), false));
}

void MyCustomGamestate::perform_logic() {
  Time new_time = Timer::get_reference().get_time();
  const float time_diff = new_time.get_seconds_since(m_time);
  m_time = new_time;

  m_angle += time_diff;

  m_a = Vector3f();
  m_b = Vector3f(segment_length * cos(m_angle), segment_length * sin(m_angle), 0.0f);
  m_collider = Collider_Type(m_a, m_b);
  
  const Point3f p0 = m_b - Vector3f(segment_length, segment_length, segment_length);
  m_parallelepiped = Parallelepiped(p0, Vector3f(2.0f * segment_length, 0.0f, 0.0f), Vector3f(0.0f, 2.0f * segment_length, 0.0f), Vector3f(0.0f, 0.0f, 2.0f * segment_length));
  
  for(Spheres::iterator it = m_spheres.begin(); it != m_spheres.end(); ++it)
    it->second = m_collider.intersects(it->first);

  for(Line_Segments::iterator it = m_line_segments.begin(); it != m_line_segments.end(); ++it)
    it->second = m_collider.intersects(it->first);

  for(Parallelepipeds::iterator it = m_parallelepipeds.begin(); it != m_parallelepipeds.end(); ++it) {
    it->second = m_collider.intersects(it->first);
    //it->second = m_parallelepiped.intersects(it->first);
  }
}

static void render_parallelepiped(const Parallelepiped &pp, const Color &color) {
  Video &vr = Video::get_reference();

  const bool ab_flip = (pp.get_edge_a() % pp.get_edge_b()) * pp.get_edge_c() < 0.0f;

  const Vector3f &a = ab_flip ? pp.get_edge_b() : pp.get_edge_a();
  const Vector3f &b = ab_flip ? pp.get_edge_a() : pp.get_edge_b();
  const Vector3f &c = pp.get_edge_c();

  const Point3f &p000 = pp.get_point();
  const Point3f p001 = p000 + c;
  const Point3f p010 = p000 + b;
  const Point3f p011 = p001 + b;
  const Point3f p100 = p000 + a;
  const Point3f p101 = p001 + a;
  const Point3f p110 = p010 + a;
  const Point3f p111 = p011 + a;

  Quadrilateral<Vertex3f_Color> qxx0(
    Vertex3f_Color(p000, -c, color),
    Vertex3f_Color(p010, -c, color),
    Vertex3f_Color(p110, -c, color),
    Vertex3f_Color(p100, -c, color));
  vr.render(qxx0);

  Quadrilateral<Vertex3f_Color> qxx1(
    Vertex3f_Color(p001, c, color),
    Vertex3f_Color(p101, c, color),
    Vertex3f_Color(p111, c, color),
    Vertex3f_Color(p011, c, color));
  vr.render(qxx1);

  Quadrilateral<Vertex3f_Color> qx0x(
    Vertex3f_Color(p000, -b, color),
    Vertex3f_Color(p100, -b, color),
    Vertex3f_Color(p101, -b, color),
    Vertex3f_Color(p001, -b, color));
  vr.render(qx0x);

  Quadrilateral<Vertex3f_Color> qx1x(
    Vertex3f_Color(p010, b, color),
    Vertex3f_Color(p011, b, color),
    Vertex3f_Color(p111, b, color),
    Vertex3f_Color(p110, b, color));
  vr.render(qx1x);

  Quadrilateral<Vertex3f_Color> q0xx(
    Vertex3f_Color(p000, -a, color),
    Vertex3f_Color(p001, -a, color),
    Vertex3f_Color(p011, -a, color),
    Vertex3f_Color(p010, -a, color));
  vr.render(q0xx);

  Quadrilateral<Vertex3f_Color> q1xx(
    Vertex3f_Color(p100, a, color),
    Vertex3f_Color(p110, a, color),
    Vertex3f_Color(p111, a, color),
    Vertex3f_Color(p101, a, color));
  vr.render(q1xx);
}

void MyCustomGamestate::render() {
  Video &vr = Video::get_reference();

  vr.set_3d(m_camera);

  for(Spheres::const_iterator it = m_spheres.begin(); it != m_spheres.end(); ++it)
    render_sphere(it->first, it->second);

  for(Parallelepipeds::iterator it = m_parallelepipeds.begin(); it != m_parallelepipeds.end(); ++it) {
    Parallelepiped &pp = it->first;
    const Color color = it->second ? Color(1.0f, 1.0f, 0.0f, 0.0f) : Color(1.0f, 0.0f, 0.0f, 1.0f);

    render_parallelepiped(pp, color);
  }

  for(Line_Segments::const_iterator it = m_line_segments.begin(); it != m_line_segments.end(); ++it) {
    const Zeni_Collision::Line_Segment &line_segment = it->first;
    const Color color = it->second ? Color(1.0f, 1.0f, 0.0f, 0.0f) : Color(1.0f, 0.0f, 0.0f, 1.0f);

    Zeni::Line_Segment<Vertex3f_Color> line_segment_r(
      Vertex3f_Color(line_segment.get_end_point_a(), color),
      Vertex3f_Color(line_segment.get_end_point_b(), color));
    vr.render(line_segment_r);
  }

  //render_parallelepiped(m_parallelepiped, Color(1.0f, 0.0f, 1.0f, 0.0f));
  Zeni::Line_Segment<Vertex3f_Color> line_segment_r(
    Vertex3f_Color(m_a, Color(1.0f, 0.0f, 1.0f, 0.0f)),
    Vertex3f_Color(m_b, Color(1.0f, 0.0f, 1.0f, 0.0f)));
  vr.render(line_segment_r);
}

void MyCustomGamestate::render_sphere(const Sphere &sphere, const bool &red) {
  Model &model = red ? m_red_sphere : m_blue_sphere;

  model.set_translate(sphere.get_center());
  model.set_scale(sphere.get_radius() * Vector3f(1.0f, 1.0f, 1.0f));

  Video::get_reference().render(model);
}

namespace Zeni {

  Gamestate_One::Gamestate_One(const vector<string> * const args) {
#ifdef GRAB_MOUSE
    SDL_WM_GrabInput(SDL_GRAB_ON);
#endif
#ifdef HIDE_MOUSE
    SDL_ShowCursor(0);
#endif
    
    if(args)
      g_args = *args;
  
    Textures::get_reference();
    Fonts::get_reference();
    Sounds::get_reference();
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    // TODO: Parse commandline arguments and bootstrap into titlestate or other gamestate;
    
    Game &gr = Game::get_reference();
    gr.pop_state();
    gr.push_state(new MyCustomGamestate());
    return;
  }

}
