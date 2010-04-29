
#ifndef ZENI_ERROR_H
#define ZENI_ERROR_H

#include <cassert>
#include <string>

namespace Zeni {

  struct Error {
    Error(const std::string &msg_) : msg(msg_) {
#ifdef _WINDOWS
      assert("Error! - Set a Breakpoint Here to Help Your Debugger Work Correctly" && false);
#endif
    }
    std::string msg;
  };
}

#endif