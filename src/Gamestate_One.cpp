/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include <zenilib.h>

#include <Zeni/Fonts.h>
#include <Zeni/Sounds.h>
#include <Zeni/Textures.h>

#include <Zeni/Sound.hxx>
#include <Zeni/Gamestate_One.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace Zeni;

static vector<string> g_args;

class Gamestate_CG : public Gamestate_Base {
#ifndef DISABLE_CG
  Vertex_Shader m_vertex_shader;
  Fragment_Shader m_fragment_shader;
#endif

  vector<Triangle<Vertex2f_Color> *> m_triangles;

  float frand() const {
    return rand() / float(RAND_MAX);
  }

  Point2f rand_point() const {
    return Point2f(800.0f * frand(),
                   600.0f * frand());
  }

  Color rand_color() const {
    return Color(frand(), frand(), frand(), frand());
  }

  Vertex2f_Color rand_vertex() const {
    return Vertex2f_Color(rand_point(), rand_color());
  }

  Triangle<Vertex2f_Color> * rand_quad() const {
    return new Triangle<Vertex2f_Color>(rand_vertex(), rand_vertex(), rand_vertex());
  }

  void render_quads(Video &vr) {
    for(vector<Triangle<Vertex2f_Color> *>::iterator it = m_triangles.begin();
        it != m_triangles.end();
        ++it)
    {
      vr.render(**it);
    }
  }

public:

  Gamestate_CG()
#ifndef DISABLE_CG
    : m_vertex_shader("shaders/zeni_vertex_shaders.cg", "Zeni_2D_Color"),
    m_fragment_shader("shaders/zeni_fragment_shaders.cg", "Zeni_Color")
#endif
  {
    m_triangles.reserve(100);
    for(int i = 0; i < 100; ++i)
      m_triangles.push_back(rand_quad());
  }
  
  ~Gamestate_CG() {
    for(vector<Triangle<Vertex2f_Color> *>::iterator it = m_triangles.begin();
        it != m_triangles.end();
        ++it)
    {
      delete *it;
    }
  }

  void perform_logic() {
  }

  void render() {
    Video &vr = get_Video();

    static struct Benchmarks {
      ~Benchmarks() {
        cout << "Fixed Function Pipeline : " << (ffp.frames / ffp.time) << " fps over " << ffp.frames << " frames" << endl;
        cout << "Cg                      : " << (cg.frames / cg.time) << " fps over " << cg.frames << " frames" << endl;
      }

      struct Benchmark {
        Benchmark()
          : time(0.0), frames(0)
        {
        }

        long double time;
        long frames;
      } ffp, cg;
    } benchmarks;

    Time_HQ before;
    Time_HQ after;

    /*** Fixed Function Pipeline ***/

    vr.set_2d_view(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)),
                   make_pair(Point2i(0, 0), Point2i(400, 600)));

    before = get_Timer_HQ().get_time();

    render_quads(vr);
    
    after = get_Timer_HQ().get_time();
    benchmarks.ffp.time += after.get_seconds_since(before);
    ++benchmarks.ffp.frames;

    /*** Cg ***/
#ifndef DISABLE_CG
    vr.set_vertex_shader(m_vertex_shader);
    vr.set_fragment_shader(m_fragment_shader);
    vr.set_viewport(make_pair(Point2i(400, 0), Point2i(800, 600)));

    before = get_Timer_HQ().get_time();

    render_quads(vr);
    
    after = get_Timer_HQ().get_time();
    benchmarks.cg.time += after.get_seconds_since(before);
    ++benchmarks.cg.frames;

    vr.unset_vertex_shader(m_vertex_shader);
    vr.unset_fragment_shader(m_fragment_shader);
#endif
  }
};

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

    get_Textures();
    get_Fonts();
    get_Sounds();
    get_Video();
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    // TODO: Parse commandline arguments and bootstrap into titlestate or other gamestate;
    
    Game &gr = get_Game();
    gr.pop_state();
    gr.push_state(new Gamestate_CG());
  }

}
