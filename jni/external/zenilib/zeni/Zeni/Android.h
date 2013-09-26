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

#ifndef ZENI_ANDROID_H
#define ZENI_ANDROID_H

#include <Zeni/String.h>

#include <iostream>
#include <cstdio>

#ifdef ANDROID

#include <android/log.h>
inline void ZENI_LOGV(const Zeni::String &str) {
  __android_log_print(ANDROID_LOG_VERBOSE, "zenilib App", "%s", str.c_str());
}
inline void ZENI_LOGD(const Zeni::String &str) {
  __android_log_print(ANDROID_LOG_DEBUG  , "zenilib App", "%s", str.c_str());
}
inline void ZENI_LOGI(const Zeni::String &str) {
  __android_log_print(ANDROID_LOG_INFO   , "zenilib App", "%s", str.c_str());
}
inline void ZENI_LOGW(const Zeni::String &str) {
  __android_log_print(ANDROID_LOG_WARN   , "zenilib App", "%s", str.c_str());
}
inline void ZENI_LOGE(const Zeni::String &str) {
  __android_log_print(ANDROID_LOG_ERROR  , "zenilib App", "%s", str.c_str());
}

#else

inline void ZENI_LOGV(const Zeni::String &str) {
  std::cerr << "zenilib App Verbose : " << str << std::endl;
}
inline void ZENI_LOGD(const Zeni::String &str) {
  std::cerr << "zenilib App Debug   : " << str << std::endl;
}
inline void ZENI_LOGI(const Zeni::String &str) {
  std::cerr << "zenilib App Info    : " << str << std::endl;
}
inline void ZENI_LOGW(const Zeni::String &str) {
  std::cerr << "zenilib App Warn    : " << str << std::endl;
}
inline void ZENI_LOGE(const Zeni::String &str) {
  std::cerr << "zenilib App Error   : " << str << std::endl;
}

#endif

#endif
