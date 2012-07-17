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

#ifdef ENABLE_SLES

namespace Zeni {

  Sound_Renderer_SLES::Sound_Renderer_SLES()
    : engineObject(NULL),
    engineEngine(NULL),
    outputMixObject(NULL)
  {
    // create engine
    if(slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL) != SL_RESULT_SUCCESS)
      throw Sound_Init_Failure();

    // realize the engine
    if((*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS)
      throw Sound_Init_Failure();

    // get the engine interface, which is needed in order to create other objects
    if((*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine) != SL_RESULT_SUCCESS)
      throw Sound_Init_Failure();

    // create output mix
    if((*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, NULL, NULL) != SL_RESULT_SUCCESS)
      throw Sound_Init_Failure();

    // realize the output mix
    if((*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS)
      throw Sound_Init_Failure();
  }

  Sound_Renderer_SLES::~Sound_Renderer_SLES() {
    // destroy output mix object, and invalidate all associated interfaces
    if(outputMixObject != NULL) {
      (*outputMixObject)->Destroy(outputMixObject);
      outputMixObject = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if(engineObject != NULL) {
      (*engineObject)->Destroy(engineObject);
      engineObject = NULL;
      engineEngine = NULL;
    }
  }

  SLEngineItf & Sound_Renderer_SLES::get_engineEngine() {
    return engineEngine;
  }

  SLObjectItf & Sound_Renderer_SLES::get_outputMixObject() {
    return outputMixObject;
  }

  void Sound_Renderer_SLES::set_listener_position(const Point3f &) {
  }

  void Sound_Renderer_SLES::set_listener_velocity(const Vector3f &) {
  }

  void Sound_Renderer_SLES::set_listener_forward_and_up(const Vector3f &, const Vector3f &) {
  }

  void Sound_Renderer_SLES::set_listener_gain(const float &gain) {
  }

  std::pair<Vector3f, Vector3f> Sound_Renderer_SLES::get_listener_forward_and_up() const {
    return std::make_pair(Vector3f(), Vector3f());
  }

}

#else

namespace Zeni {
  void * this_pointer_is_silent_sound_too = (void *)0xDEADBEEF;
}

#endif
