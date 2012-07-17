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

#ifndef ZENI_SOUND_RENDERER_AL_H
#define ZENI_SOUND_RENDERER_AL_H

#include <Zeni/Sound_Renderer.h>

#ifndef DISABLE_AL

#ifdef _MACOSX
#include <OpenAL/alc.h>
#else
#include <AL/alc.h>
#endif

#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#undef OV_EXCLUDE_STATIC_CALLBACKS

#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace Zeni {

  class ZENI_AUDIO_DLL Sound_Renderer_AL : public Sound_Renderer {
    friend class Sound;

    Sound_Renderer_AL();
    ~Sound_Renderer_AL();

    // Undefined
    Sound_Renderer_AL(const Sound_Renderer_AL &);
    Sound_Renderer_AL & operator=(const Sound_Renderer_AL &);

  public:
    typedef void (AL_APIENTRY *alBufferData_fcn)( ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq );
    typedef ALCboolean (ALC_APIENTRY *alcCloseDevice_fcn)( ALCdevice *device );
    typedef ALCcontext * (ALC_APIENTRY *alcCreateContext_fcn)( ALCdevice *device, const ALCint* attrlist );
    typedef void (ALC_APIENTRY *alcDestroyContext_fcn)( ALCcontext *context );
    typedef ALboolean (AL_APIENTRY *alIsExtensionPresent_fcn)( const ALchar* extname );
    typedef ALCboolean (ALC_APIENTRY *alcMakeContextCurrent_fcn)( ALCcontext *context );
    typedef ALCdevice * (ALC_APIENTRY *alcOpenDevice_fcn)( const ALCchar *devicename );
    typedef void (AL_APIENTRY *alDeleteBuffers_fcn)( ALsizei n, const ALuint* buffers );
    typedef void (AL_APIENTRY *alDeleteSources_fcn)( ALsizei n, const ALuint* sources );
    typedef void (AL_APIENTRY *alGenBuffers_fcn)( ALsizei n, ALuint* buffers );
    typedef ALenum (AL_APIENTRY *alGetError_fcn)( void );
    typedef void (AL_APIENTRY *alGetListenerf_fcn)( ALenum param, ALfloat* value );
    typedef void (AL_APIENTRY *alGetListenerfv_fcn)( ALenum param, ALfloat* values );
    typedef void (AL_APIENTRY *alGetSourcef_fcn)( ALuint sid, ALenum param, ALfloat* value );
    typedef void (AL_APIENTRY *alGetSourcefv_fcn)( ALuint sid, ALenum param, ALfloat* values );
    typedef void (AL_APIENTRY *alGetSourcei_fcn)( ALuint sid,  ALenum param, ALint* value );
    typedef void (AL_APIENTRY *alGenSources_fcn)( ALsizei n, ALuint* sources );
    typedef void (AL_APIENTRY *alListenerf_fcn)( ALenum param, ALfloat value );
    typedef void (AL_APIENTRY *alListenerfv_fcn)( ALenum param, const ALfloat* values );
    typedef void (AL_APIENTRY *alSourcef_fcn)( ALuint sid, ALenum param, ALfloat value );
    typedef void (AL_APIENTRY *alSourcefv_fcn)( ALuint sid, ALenum param, const ALfloat* values );
    typedef void (AL_APIENTRY *alSourcei_fcn)( ALuint sid, ALenum param, ALint value );
    typedef void (AL_APIENTRY *alSourcePause_fcn)( ALuint sid );
    typedef void (AL_APIENTRY *alSourcePlay_fcn)( ALuint sid );
    typedef void (AL_APIENTRY *alSourceStop_fcn)( ALuint sid );

    // DLL Functions
    static alBufferData_fcn alBufferData() {return g_alBufferData;}
    static alcCloseDevice_fcn alcCloseDevice() {return g_alcCloseDevice;}
    static alcCreateContext_fcn alcCreateContext() {return g_alcCreateContext;}
    static alcDestroyContext_fcn alcDestroyContext() {return g_alcDestroyContext;}
    static alIsExtensionPresent_fcn alIsExtensionPresent() {return g_alIsExtensionPresent;}
    static alcMakeContextCurrent_fcn alcMakeContextCurrent() {return g_alcMakeContextCurrent;}
    static alcOpenDevice_fcn alcOpenDevice() {return g_alcOpenDevice;}
    static alDeleteBuffers_fcn alDeleteBuffers() {return g_alDeleteBuffers;}
    static alDeleteSources_fcn alDeleteSources() {return g_alDeleteSources;}
    static alGenBuffers_fcn alGenBuffers() {return g_alGenBuffers;}
    static alGetError_fcn alGetError() {return g_alGetError;}
    static alGetListenerf_fcn alGetListenerf() {return g_alGetListenerf;}
    static alGetListenerfv_fcn alGetListenerfv() {return g_alGetListenerfv;}
    static alGetSourcef_fcn alGetSourcef() {return g_alGetSourcef;}
    static alGetSourcefv_fcn alGetSourcefv() {return g_alGetSourcefv;}
    static alGetSourcei_fcn alGetSourcei() {return g_alGetSourcei;}
    static alGenSources_fcn alGenSources() {return g_alGenSources;}
    static alListenerf_fcn alListenerf() {return g_alListenerf;}
    static alListenerfv_fcn alListenerfv() {return g_alListenerfv;}
    static alSourcef_fcn alSourcef() {return g_alSourcef;}
    static alSourcefv_fcn alSourcefv() {return g_alSourcefv;}
    static alSourcei_fcn alSourcei() {return g_alSourcei;}
    static alSourcePause_fcn alSourcePause() {return g_alSourcePause;}
    static alSourcePlay_fcn alSourcePlay() {return g_alSourcePlay;}
    static alSourceStop_fcn alSourceStop() {return g_alSourceStop;}

    static String errorString(const ALenum &err);
    static String errorString();

    // Listener Functions
    void set_listener_position(const Point3f &position); ///< Set the position of the listener and BGM.
    void set_listener_velocity(const Vector3f &velocity); ///< Set the velocity of the listener and BGM for the doppler effect.
    void set_listener_forward_and_up(const Vector3f &forward, const Vector3f &up); ///< Set the orientation of the listener
    void set_listener_gain(const float &gain); ///< Set the listener gain

    std::pair<Vector3f, Vector3f> get_listener_forward_and_up() const; ///< Set the orientation of the listener

  private:
    void zero_handles();

#ifndef _WINDOWS
    typedef void * HMODULE;
#endif

    HMODULE m_openal32;
    static alBufferData_fcn g_alBufferData;
    static alcCloseDevice_fcn g_alcCloseDevice;
    static alcCreateContext_fcn g_alcCreateContext;
    static alcDestroyContext_fcn g_alcDestroyContext;
    static alIsExtensionPresent_fcn g_alIsExtensionPresent;
    static alcMakeContextCurrent_fcn g_alcMakeContextCurrent;
    static alcOpenDevice_fcn g_alcOpenDevice;
    static alDeleteBuffers_fcn g_alDeleteBuffers;
    static alDeleteSources_fcn g_alDeleteSources;
    static alGenBuffers_fcn g_alGenBuffers;
    static alGetError_fcn g_alGetError;
    static alGetListenerf_fcn g_alGetListenerf;
    static alGetListenerfv_fcn g_alGetListenerfv;
    static alGetSourcef_fcn g_alGetSourcef;
    static alGetSourcefv_fcn g_alGetSourcefv;
    static alGetSourcei_fcn g_alGetSourcei;
    static alGenSources_fcn g_alGenSources;
    static alListenerf_fcn g_alListenerf;
    static alListenerfv_fcn g_alListenerfv;
    static alSourcef_fcn g_alSourcef;
    static alSourcefv_fcn g_alSourcefv;
    static alSourcei_fcn g_alSourcei;
    static alSourcePause_fcn g_alSourcePause;
    static alSourcePlay_fcn g_alSourcePlay;
    static alSourceStop_fcn g_alSourceStop;

    ALCdevice *m_device;
    ALCcontext *m_context;

    String m_bgmusic;
    Sound_Buffer *m_bgm;
    Sound_Source *m_bgm_source;
  };

}

#endif

#endif
