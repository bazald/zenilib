#include <zenilib.h>

#include <jni.h>
#include <errno.h>

#include <GLES/gl.h>

#include <android/sensor.h>
#include <android_native_app_glue.h>

#include <android/window.h>
#include <cmath>
#include <utility>

using namespace Zeni;

/**
 * Shared state for our app.
 */
struct engine {
  struct android_app* app;

  ASensorManager* sensorManager;
  const ASensor* accelerometerSensor;
  ASensorEventQueue* sensorEventQueue;
};

class Play_State : public Gamestate_Base {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  struct Saved_State {
    Saved_State() {}
    ~Saved_State() {}
  public:
    static Saved_State & get() {
      static Saved_State ss;
      return ss;
    }

    Point2f position;
    Vector2f velocity;
    Vector3f acceleration;
  };

  Play_State() {
#ifndef ANDROID
    set_pausable(true);
#endif
  }

private:
#ifdef ANDROID
  void on_event(android_app * const &app, const AInputEvent * const &event) {
  }
#else
  void on_push() {
    get_Window().mouse_hide(true);
  }

  void on_pop() {
    get_Window().mouse_hide(false);
  }

  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_SPACE) {
      if(event.state == SDL_PRESSED)
        play_sound("hello_world");
    }
    else
      Gamestate_Base::on_key(event);
  }
#endif

  void prerender() {
    if(!get_Textures().find("sson")) {
      Image image(Point2i(2, 2), Image::Color_Space::RGB, true);
      image.set_Color(Point2i(), Color(1.0f, 1.0f, 0.0f, 0.0f));
      image.set_Color(Point2i(0, 1), Color(1.0f, 0.0f, 1.0f, 0.0f));
      image.set_Color(Point2i(1, 0), Color(1.0f, 0.0f, 0.0f, 1.0f));
      image.set_Color(Point2i(1, 1), Color());
      get_Textures().give("sson", get_Video().create_Texture(image), false);
    }

    if(!get_Textures().find("ssoff")) {
      Image image(Point2i(2, 2), Image::Color_Space::Luminance, true);
      image.set_Color(Point2i(), Color());
      image.set_Color(Point2i(0, 1), Color(1.0f, 0.0f, 0.0f, 0.0f));
      image.set_Color(Point2i(1, 0), Color(1.0f, 0.0f, 0.0f, 0.0f));
      image.set_Color(Point2i(1, 1), Color());
      get_Textures().give("ssoff", get_Video().create_Texture(image), false);
    }
  }

  void render() {
    Window &wr = get_Window();

    glViewport(0, 0, wr.get_width(), wr.get_height());

    // Just fill the screen with a color.
    glClear(GL_COLOR_BUFFER_BIT);



    const float w = 600.0f * wr.get_width() / wr.get_height();

//     get_Video().set_2d(make_pair(Point2f(), Point2f(w, 600.0f)));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0.0f, w, 600.0f, 0.0f, -1.0f, 1.0f);

    const float w2 = w / 2.0f;
    const float h2 = 300.0f;

    for(int i = 0; i != 4; ++i) {
      const float x = i & 1 ? w2 : 0.0f;
      const float y = i / 2 ? h2 : 0.0f;

      Vertex2f_Texture v0(Point2f(x, y), Point2f());
      Vertex2f_Texture v1(Point2f(x, y + h2), Point2f(0.0f, 1.0f));
      Vertex2f_Texture v2(Point2f(x + w2, y + h2), Point2f(1.0f, 1.0f));
      Vertex2f_Texture v3(Point2f(x + w2, y), Point2f(1.0f, 0.0f));

      Quadrilateral<Vertex2f_Texture> quad(v0, v1, v2, v3);
      Material mat("blarg");
      quad.lend_Material(&mat);

      set_sprite_frame("blarg", i);
      get_Video().render(quad);
    }

#ifdef ANDROID
    {
      const bool ss = get_Core().is_screen_saver_enabled();
      const int x = int(Saved_State::get().position.x) & 0xFFFFFFFE;
      const int y = int(Saved_State::get().position.y) & 0xFFFFFFFE;
      const float tx = 0.05f * w;
      const float ty = 0.05f * 600.0f;
      const float w10 = w / 10.0f;

      Vertex2f_Texture v0(Point2f(x, y), Point2f());
      Vertex2f_Texture v1(Point2f(x, y + 60.0f), Point2f(0.0f, ty));
      Vertex2f_Texture v2(Point2f(x + w10, y + 60.0f), Point2f(tx, ty));
      Vertex2f_Texture v3(Point2f(x + w10, y), Point2f(tx, 0.0f));

      Quadrilateral<Vertex2f_Texture> quad(v0, v1, v2, v3);
      Material mat(ss ? "sson" : "ssoff");
      quad.lend_Material(&mat);

      get_Video().render(quad);
    }
#endif

    get_Fonts()["system_36_x600"].render_text("Hello world!\nThis is\nthe event\nmankind\nhas been\nwaiting for.", Point2f(), Color(1.0f, 1.0f, 0.0f, 1.0f));



    eglSwapBuffers(get_Window().get_display(), get_Window().get_surface());
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      return new Play_State;
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
  get_Window();

  // Initialize GL state.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glDisable(GL_DEPTH_TEST);

  return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* /*engine*/) {
  /// Excerpt from Game::run(...)
  if(Window::is_enabled()) {
    Video &vr = get_Video();

    if(vr.begin_prerender()) {
      get_Game().prerender();

      if(vr.begin_render()) {
        try {
          get_Game().render();
        }
        catch(...) {
          vr.end_render();
          throw;
        }

        vr.end_render();
      }
    }
  }
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
  Game::completely_destroy();
#ifndef TEMP_DISABLE
  Net::completely_destroy();
#endif
  Fonts::completely_destroy();
  Textures::completely_destroy();
  Video::completely_destroy();
  Window::completely_destroy();
  Sound_Source_Pool::destroy();
  Sounds::completely_destroy();
  Sound::completely_destroy();
  Core::completely_destroy();
  Joysticks::completely_destroy();
  Timer::completely_destroy();
  Colors::completely_destroy();
  File_Ops::completely_destroy();
  Resource::completely_destroy();
  Timer_HQ::completely_destroy();

  Window::set_enabled(false);
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
  switch (AInputEvent_getType(event)) {
    case AINPUT_EVENT_TYPE_KEY:
      switch(AKeyEvent_getKeyCode(event)) {
        case AKEYCODE_MENU:
          if(AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN)
            get_Core().set_screen_saver(!get_Core().is_screen_saver_enabled());
          else {
            ZENI_LOGI("<audio>Hello World, ...</audio>");
            play_sound("hello_world");
            get_Sound_Source_Pool().update();
          }
          return 1;

        case AKEYCODE_BACK:
          break;

        case AKEYCODE_SEARCH:
          break;

        default:
          break;
      }
    break;

    case AINPUT_EVENT_TYPE_MOTION:
      Play_State::Saved_State::get().position.x = AMotionEvent_getX(event, 0);
      Play_State::Saved_State::get().position.y = AMotionEvent_getY(event, 0);
      Play_State::Saved_State::get().velocity.x = 0.0f;
      Play_State::Saved_State::get().velocity.y = 0.0f;
      return 1;

    default:
      break;
  }

  return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
#ifndef NDEBUG
  return;
#endif

  struct engine* engine = (struct engine*)app->userData;
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      ZENI_LOGI("APP_CMD_INIT_WINDOW");
      // The window is being shown, get it ready.
      if (engine->app->window != NULL) {
        engine_init_display(engine);
        engine_draw_frame(engine);
      }
      break;
    case APP_CMD_GAINED_FOCUS:
      ZENI_LOGI("APP_CMD_GAINED_FOCUS");
      // When our app gains focus, we start monitoring the accelerometer.
      if(engine->accelerometerSensor) {
        ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                                       engine->accelerometerSensor);
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                                       engine->accelerometerSensor,
                                       (1000L/60)*1000);
      }
      Window::set_enabled(true);
      break;
    case APP_CMD_LOST_FOCUS:
      ZENI_LOGI("APP_CMD_LOST_FOCUS");
      // When our app loses focus, we stop monitoring the accelerometer.
      // This is to avoid consuming battery while not being used.
      if(engine->accelerometerSensor) {
        ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                                        engine->accelerometerSensor);
      }
      // Also stop animating.
      Window::set_enabled(false);
      break;
    case APP_CMD_TERM_WINDOW:
      ZENI_LOGI("APP_CMD_TERM_WINDOW");
      // The window is being hidden or closed, clean it up.
      engine_term_display(engine);
      break;
    case APP_CMD_SAVE_STATE:
      // The system has asked us to save our current state.  Do so.
      ZENI_LOGI("APP_CMD_SAVE_STATE");
      engine->app->savedStateSize = sizeof(Play_State::Saved_State);
      engine->app->savedState = malloc(engine->app->savedStateSize);
      *reinterpret_cast<Play_State::Saved_State *>(engine->app->savedState) = Play_State::Saved_State::get();
      break;
  }
}

#ifndef NDEBUG
bool g_ndk = false;
#endif

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
  // Make sure glue isn't stripped.
  app_dummy();

#ifndef NDEBUG
  while(!g_ndk) {
    // Read all pending events.
    int ident;
    int events;
    struct android_poll_source* source;
    while((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
      if(source)
        source->process(state, source);
    }
  }
  /// set g_ndk=1
  raise(SIGINT);
#endif

  Core::preinit(state);

  struct engine engine;

  // Keep the screen on.
  get_Core().set_screen_saver(false);

  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;

  // Prepare to monitor accelerometer
  engine.sensorManager = ASensorManager_getInstance();
  engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
                                                               ASENSOR_TYPE_ACCELEROMETER);
  engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
                                                            state->looper,
                                                            LOOPER_ID_USER,
                                                            NULL,
                                                            NULL);

  Play_State::Saved_State &saved_state = Play_State::Saved_State::get();
  if(state->savedState) {
    // We are starting with a previous saved state; restore from it.
    saved_state = *reinterpret_cast<Play_State::Saved_State *>(state->savedState);
  }

  // loop waiting for stuff to do.

  while (1) {
    // Read all pending events.
    int ident;
    int events;
    struct android_poll_source* source;

    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    const int block = 0; // 0 == no, -1 == yes
    while((ident=ALooper_pollAll(block, NULL, &events, (void**)&source)) >= 0)
    {
      // Process this event.
      if (source != NULL) {
          source->process(state, source);
      }

      // If a sensor has data, process it now.
      if (ident == LOOPER_ID_USER) {
          if (engine.accelerometerSensor != NULL) {
              ASensorEvent event;
              while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                      &event, 1) > 0) {
                  // LOGI("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
                  saved_state.acceleration.x = event.acceleration.x;
                  saved_state.acceleration.y = event.acceleration.y;
                  saved_state.acceleration.z = event.acceleration.z;
              }
          }
      }

      // Check if we are exiting.
      if(state->destroyRequested) {
        engine_term_display(&engine);
        return;
      }
    }

    {
      // Do updates
      saved_state.velocity.x -= saved_state.acceleration.x;
      saved_state.velocity.y += saved_state.acceleration.y;

      const float w10 = get_Window().get_width() / 10.0f;
      const float h10 = get_Window().get_height() / 10.0f;

      const float v = saved_state.velocity.magnitude();

      const float mu0 = 1.5f;
      const float mu = 0.5f;
      const float mu_actual = v < 0.001f ? mu0 : mu;
      const float n = mu_actual * saved_state.acceleration.z;

      if(v < n)
        saved_state.velocity = Vector2f();
      else if(n > 0.0f) {
        const float fraction = 1.0f - n / v;
        saved_state.velocity *= fraction;
      }

      saved_state.position += saved_state.velocity;

      if(saved_state.position.x < 0.0f) {
        saved_state.position.x = 0.0f;
        saved_state.velocity.x = std::max(0.0f, saved_state.velocity.x);
      }
      else if(saved_state.position.x + w10 >= get_Window().get_width()) {
        saved_state.position.x = get_Window().get_width() - w10;
        saved_state.velocity.x = std::min(0.0f, saved_state.velocity.x);
      }
      if(saved_state.position.y < 0.0f) {
        saved_state.position.y = 0.0f;
        saved_state.velocity.y = std::max(0.0f, saved_state.velocity.y);
      }
      else if(saved_state.position.y + h10 >= get_Window().get_height()) {
        saved_state.position.y = get_Window().get_height() - h10;
        saved_state.velocity.y = std::min(0.0f, saved_state.velocity.y);
      }

      // Drawing is throttled to the screen update rate, so there
      // is no need to do timing here.
      engine_draw_frame(&engine);
    }
  }
}
