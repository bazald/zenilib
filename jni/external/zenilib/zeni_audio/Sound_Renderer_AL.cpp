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

#include <zeni_audio.h>

#include <iostream>
#include <vector>
#include <iomanip>

#ifndef _WINDOWS
#include <dlfcn.h>
#endif

#ifndef DISABLE_AL

#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#undef OV_EXCLUDE_STATIC_CALLBACKS

namespace Zeni {

#ifdef _LINUX
  static void * LoadLibrary(const char * const &file) {
    return dlopen(file, RTLD_LAZY);
  }

  static int FreeLibrary(void * const &handle) {
    return dlclose(handle);
  }

  static void * GetProcAddress(void * const &handle, const char * const & name) {
    return dlsym(handle, name);
  }
#endif

  Sound_Renderer_AL::Sound_Renderer_AL()
    : m_device(0),
    m_context(0),
    m_bgm(0),
    m_bgm_source(0)
  {
#ifndef _MACOSX
#ifdef _WINDOWS
    m_openal32 = LoadLibrary("OpenAL32.dll");
#else
    m_openal32 = LoadLibrary("libopenal.so");
    if(!m_openal32)
      m_openal32 = LoadLibrary("libopenal.so.1");
#endif
    if(!m_openal32) {
      std::cerr << "Loading OpenAL32.dll failed." << std::endl;

      throw Sound_Init_Failure();
    }

    g_alBufferData = (alBufferData_fcn)GetProcAddress(m_openal32, "alBufferData");
    g_alcCloseDevice = (alcCloseDevice_fcn)GetProcAddress(m_openal32, "alcCloseDevice");
    g_alcCreateContext = (alcCreateContext_fcn)GetProcAddress(m_openal32, "alcCreateContext");
    g_alcDestroyContext = (alcDestroyContext_fcn)GetProcAddress(m_openal32, "alcDestroyContext");
    g_alIsExtensionPresent = (alIsExtensionPresent_fcn)GetProcAddress(m_openal32, "alIsExtensionPresent");
    g_alcMakeContextCurrent = (alcMakeContextCurrent_fcn)GetProcAddress(m_openal32, "alcMakeContextCurrent");
    g_alcOpenDevice = (alcOpenDevice_fcn)GetProcAddress(m_openal32, "alcOpenDevice");
    g_alDeleteBuffers = (alDeleteBuffers_fcn)GetProcAddress(m_openal32, "alDeleteBuffers");
    g_alDeleteSources = (alDeleteSources_fcn)GetProcAddress(m_openal32, "alDeleteSources");
    g_alGenBuffers = (alGenBuffers_fcn)GetProcAddress(m_openal32, "alGenBuffers");
    g_alGetError = (alGetError_fcn)GetProcAddress(m_openal32, "alGetError");
    g_alGetListenerf = (alGetListenerf_fcn)GetProcAddress(m_openal32, "alGetListenerf");
    g_alGetListenerfv = (alGetListenerfv_fcn)GetProcAddress(m_openal32, "alGetListenerfv");
    g_alGetSourcef = (alGetSourcef_fcn)GetProcAddress(m_openal32, "alGetSourcef");
    g_alGetSourcefv = (alGetSourcefv_fcn)GetProcAddress(m_openal32, "alGetSourcefv");
    g_alGetSourcei = (alGetSourcei_fcn)GetProcAddress(m_openal32, "alGetSourcei");
    g_alGenSources = (alGenSources_fcn)GetProcAddress(m_openal32, "alGenSources");
    g_alListenerf = (alListenerf_fcn)GetProcAddress(m_openal32, "alListenerf");
    g_alListenerfv = (alListenerfv_fcn)GetProcAddress(m_openal32, "alListenerfv");
    g_alSourcef = (alSourcef_fcn)GetProcAddress(m_openal32, "alSourcef");
    g_alSourcefv = (alSourcefv_fcn)GetProcAddress(m_openal32, "alSourcefv");
    g_alSourcei = (alSourcei_fcn)GetProcAddress(m_openal32, "alSourcei");
    g_alSourcePause = (alSourcePause_fcn)GetProcAddress(m_openal32, "alSourcePause");
    g_alSourcePlay = (alSourcePlay_fcn)GetProcAddress(m_openal32, "alSourcePlay");
    g_alSourceStop = (alSourceStop_fcn)GetProcAddress(m_openal32, "alSourceStop");
    if(!g_alBufferData || !g_alcCloseDevice || !g_alcCreateContext || !g_alcDestroyContext ||
       !g_alIsExtensionPresent || !g_alcMakeContextCurrent || !g_alcOpenDevice || !g_alDeleteBuffers ||
       !g_alDeleteSources || !g_alGenBuffers || !g_alGetError || !g_alGetListenerf ||
       !g_alGetListenerfv || !g_alGetSourcef || !g_alGetSourcefv || !g_alGetSourcei ||
       !g_alGenSources || !g_alListenerf || !g_alListenerfv || !g_alSourcef ||
       !g_alSourcefv || !g_alSourcei || !g_alSourcePause || !g_alSourcePlay ||
       !g_alSourceStop)
    {
      std::cerr << "Loading OpenAL32.dll failed." << std::endl;

      zero_handles();
      FreeLibrary(m_openal32);

      throw Sound_Init_Failure();
    }
#else
    g_alBufferData = (alBufferData_fcn)::alBufferData;
    g_alcCloseDevice = (alcCloseDevice_fcn)::alcCloseDevice;
    g_alcCreateContext = (alcCreateContext_fcn)::alcCreateContext;
    g_alcDestroyContext = (alcDestroyContext_fcn)::alcDestroyContext;
    g_alIsExtensionPresent = (alIsExtensionPresent_fcn)::alIsExtensionPresent;
    g_alcMakeContextCurrent = (alcMakeContextCurrent_fcn)::alcMakeContextCurrent;
    g_alcOpenDevice = (alcOpenDevice_fcn)::alcOpenDevice;
    g_alDeleteBuffers = (alDeleteBuffers_fcn)::alDeleteBuffers;
    g_alDeleteSources = (alDeleteSources_fcn)::alDeleteSources;
    g_alGenBuffers = (alGenBuffers_fcn)::alGenBuffers;
    g_alGetError = (alGetError_fcn)::alGetError;
    g_alGetListenerf = (alGetListenerf_fcn)::alGetListenerf;
    g_alGetListenerfv = (alGetListenerfv_fcn)::alGetListenerfv;
    g_alGetSourcef = (alGetSourcef_fcn)::alGetSourcef;
    g_alGetSourcefv = (alGetSourcefv_fcn)::alGetSourcefv;
    g_alGetSourcei = (alGetSourcei_fcn)::alGetSourcei;
    g_alGenSources = (alGenSources_fcn)::alGenSources;
    g_alListenerf = (alListenerf_fcn)::alListenerf;
    g_alListenerfv = (alListenerfv_fcn)::alListenerfv;
    g_alSourcef = (alSourcef_fcn)::alSourcef;
    g_alSourcefv = (alSourcefv_fcn)::alSourcefv;
    g_alSourcei = (alSourcei_fcn)::alSourcei;
    g_alSourcePause = (alSourcePause_fcn)::alSourcePause;
    g_alSourcePlay = (alSourcePlay_fcn)::alSourcePlay;
    g_alSourceStop = (alSourceStop_fcn)::alSourceStop;
#endif

    m_device = alcOpenDevice()(0);
    if(!m_device) {
      zero_handles();
#ifndef _MACOSX
      FreeLibrary(m_openal32);
#endif

      throw Sound_Init_Failure();
    }

    m_context = alcCreateContext()(m_device, 0);
    if(!m_context) {
      alcCloseDevice()(m_device);

      zero_handles();
#ifndef _MACOSX
      FreeLibrary(m_openal32);
#endif

      throw Sound_Init_Failure();
    }

    if(!alcMakeContextCurrent()(m_context)) {
      alcDestroyContext()(m_context);
      alcCloseDevice()(m_device);

      zero_handles();
#ifndef _MACOSX
      FreeLibrary(m_openal32);
#endif

      throw Sound_Init_Failure();
    }

    // Check for Vorbis extension functionality; seems to always fail :(
    alIsExtensionPresent()("AL_EXT_vorbis");
    //cerr << "Valid Audio Formats: " << alutGetMIMETypes(ALUT_LOADER_BUFFER) << std::endl;

    ALfloat listener_position[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_velocity[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_forward_and_up[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    alListenerfv()(AL_POSITION, listener_position);
    alListenerfv()(AL_VELOCITY, listener_velocity);
    alListenerfv()(AL_ORIENTATION, listener_forward_and_up);
  }

  Sound_Renderer_AL::~Sound_Renderer_AL() {
    alcDestroyContext()(m_context);
    alcCloseDevice()(m_device);

#ifndef _MACOSX
    FreeLibrary(m_openal32);
#endif

    zero_handles();
  }

  String Sound_Renderer_AL::errorString(const ALenum &err) {
    switch(err) {
      case AL_NO_ERROR:          return "AL_NO_ERROR";          break;
      case AL_INVALID_NAME:      return "AL_INVALID_NAME";      break;
      case AL_INVALID_ENUM:      return "AL_INVALID_ENUM";      break;
      case AL_INVALID_VALUE:     return "AL_INVALID_VALUE";     break;
      case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION"; break;
      case AL_OUT_OF_MEMORY:     return "AL_OUT_OF_MEMORY";     break;
      default:                   return "AL_UNKNOWN_ERROR";     break;
    }
  }

  String Sound_Renderer_AL::errorString() {
    if(Sound_Renderer_AL::alGetError())
      return errorString(Sound_Renderer_AL::alGetError()());
    else
      return "OpenAL not initialized";
  }

  void Sound_Renderer_AL::set_listener_position(const Point3f &position) {
    ALfloat listener_position[3] = {position.x, position.y, position.z};
    alListenerfv()(AL_POSITION, listener_position);
  }

  void Sound_Renderer_AL::set_listener_velocity(const Vector3f &velocity) {
    ALfloat listener_velocity[3] = {velocity.i, velocity.j, velocity.k};
    alListenerfv()(AL_VELOCITY, listener_velocity);
  }

  void Sound_Renderer_AL::set_listener_forward_and_up(const Vector3f &forward, const Vector3f &up) {
    ALfloat listener_forward_and_up[6] = {forward.i, forward.j, forward.k, up.i, up.j, up.k};
    alListenerfv()(AL_ORIENTATION, listener_forward_and_up);
  }

  void Sound_Renderer_AL::set_listener_gain(const float &gain) {
    alListenerf()(AL_GAIN, gain);
  }

  std::pair<Vector3f, Vector3f> Sound_Renderer_AL::get_listener_forward_and_up() const {
    ALfloat lfau[6] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    alGetListenerfv()(AL_ORIENTATION, lfau);
    return std::make_pair(Vector3f(lfau[0], lfau[1], lfau[2]), Vector3f(lfau[3], lfau[4], lfau[5]));
  }

  void Sound_Renderer_AL::zero_handles() {
      g_alBufferData = 0;
      g_alcCloseDevice = 0;
      g_alcCreateContext = 0;
      g_alcDestroyContext = 0;
      g_alIsExtensionPresent = 0;
      g_alcMakeContextCurrent = 0;
      g_alcOpenDevice = 0;
      g_alDeleteBuffers = 0;
      g_alDeleteSources = 0;
      g_alGenBuffers = 0;
      g_alGetError = 0;
      g_alGetListenerf = 0;
      g_alGetListenerfv = 0;
      g_alGetSourcef = 0;
      g_alGetSourcefv = 0;
      g_alGetSourcei = 0;
      g_alGenSources = 0;
      g_alListenerf = 0;
      g_alListenerfv = 0;
      g_alSourcef = 0;
      g_alSourcefv = 0;
      g_alSourcei = 0;
      g_alSourcePause = 0;
      g_alSourcePlay = 0;
      g_alSourceStop = 0;
  }

  Sound_Renderer_AL::alBufferData_fcn Sound_Renderer_AL::g_alBufferData = 0;
  Sound_Renderer_AL::alcCloseDevice_fcn Sound_Renderer_AL::g_alcCloseDevice = 0;
  Sound_Renderer_AL::alcCreateContext_fcn Sound_Renderer_AL::g_alcCreateContext = 0;
  Sound_Renderer_AL::alcDestroyContext_fcn Sound_Renderer_AL::g_alcDestroyContext = 0;
  Sound_Renderer_AL::alIsExtensionPresent_fcn Sound_Renderer_AL::g_alIsExtensionPresent = 0;
  Sound_Renderer_AL::alcMakeContextCurrent_fcn Sound_Renderer_AL::g_alcMakeContextCurrent = 0;
  Sound_Renderer_AL::alcOpenDevice_fcn Sound_Renderer_AL::g_alcOpenDevice = 0;
  Sound_Renderer_AL::alDeleteBuffers_fcn Sound_Renderer_AL::g_alDeleteBuffers = 0;
  Sound_Renderer_AL::alDeleteSources_fcn Sound_Renderer_AL::g_alDeleteSources = 0;
  Sound_Renderer_AL::alGenBuffers_fcn Sound_Renderer_AL::g_alGenBuffers = 0;
  Sound_Renderer_AL::alGetError_fcn Sound_Renderer_AL::g_alGetError = 0;
  Sound_Renderer_AL::alGetListenerf_fcn Sound_Renderer_AL::g_alGetListenerf = 0;
  Sound_Renderer_AL::alGetListenerfv_fcn Sound_Renderer_AL::g_alGetListenerfv = 0;
  Sound_Renderer_AL::alGetSourcef_fcn Sound_Renderer_AL::g_alGetSourcef = 0;
  Sound_Renderer_AL::alGetSourcefv_fcn Sound_Renderer_AL::g_alGetSourcefv = 0;
  Sound_Renderer_AL::alGetSourcei_fcn Sound_Renderer_AL::g_alGetSourcei = 0;
  Sound_Renderer_AL::alGenSources_fcn Sound_Renderer_AL::g_alGenSources = 0;
  Sound_Renderer_AL::alListenerf_fcn Sound_Renderer_AL::g_alListenerf = 0;
  Sound_Renderer_AL::alListenerfv_fcn Sound_Renderer_AL::g_alListenerfv = 0;
  Sound_Renderer_AL::alSourcef_fcn Sound_Renderer_AL::g_alSourcef = 0;
  Sound_Renderer_AL::alSourcefv_fcn Sound_Renderer_AL::g_alSourcefv = 0;
  Sound_Renderer_AL::alSourcei_fcn Sound_Renderer_AL::g_alSourcei = 0;
  Sound_Renderer_AL::alSourcePause_fcn Sound_Renderer_AL::g_alSourcePause = 0;
  Sound_Renderer_AL::alSourcePlay_fcn Sound_Renderer_AL::g_alSourcePlay = 0;
  Sound_Renderer_AL::alSourceStop_fcn Sound_Renderer_AL::g_alSourceStop = 0;

}

#else

namespace Zeni {
  void * this_pointer_is_silent_sound = (void *)0xDEADBEEF;
}

#endif
