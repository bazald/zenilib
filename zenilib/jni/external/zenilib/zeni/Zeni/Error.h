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
 * \struct Zeni::Error
 *
 * \ingroup zenilib
 *
 * \brief The Error Class
 *
 * A simple class for throwing an error message.
 *
 * \note This is also a base class for all error messages in zenilib.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_ERROR_H
#define ZENI_ERROR_H

#include <Zeni/String.h>

#include <cassert>
#include <stdexcept>

#if defined(_MACOSX)
#include <Zeni/macosx_zeni.h>
#elif defined(_WINDOWS)
#include <Windows.h>
#elif defined(ANDROID)
#include <android/log.h>
#endif

namespace Zeni {

  struct ZENI_DLL Error : public std::exception {
    Error(const String &msg_) : msg(msg_) {
#ifdef _WINDOWS
      assert("Error! - Set a Breakpoint Here to Make Your Debugger More Useful" && false);
#endif
    }

    ~Error() throw() {}

    String msg;
  };

  inline void message_box(const char * const &msg) {
#if defined(_MACOSX)
    mac_message_box(msg);
#elif defined(_WINDOWS)
    MessageBoxA(0, msg, 0, MB_OK);
#elif defined(ANDROID)
    __android_log_print(ANDROID_LOG_VERBOSE, "zenilib", "%s", msg);
#else
    String cmd("xmessage -center '");
    for(const char * cc = msg; *cc; ++cc) {
      if(*cc != '\'')
        cmd += *cc;
      else
        cmd += "'\"'\"'";
    }
    cmd += '\'';
    system(cmd.c_str());
#endif
  }

  inline void message_box(const String &msg) {
    message_box(msg.c_str());
  }

}

#endif
