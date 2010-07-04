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

/**
 * \class Zeni::Random
 *
 * \ingroup zenilib
 *
 * \brief A Random Number Generator
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_RANDOM_H
#define ZENI_RANDOM_H

#include <cassert>
#include <cstdlib>

#include <SDL/SDL_stdinc.h>

namespace Zeni {

  class Random {
  public:
    Random(const Uint32 &seed = std::rand())
     : m_random_value(seed)
    {
    }

    /// Get the maximum size of a random integer returned from rand()
    Sint32 rand_max() const {
      return 32767;
    }

    /// Get a random integer in the range [0, rand_max()]
    Sint32 rand() {
      m_random_value = m_random_value * 1103515245 + 12345;
      return Sint32(static_cast<Uint32>(m_random_value / 65536) % (rand_max() + 1));
    }

    /// Get a random floating point number in the range [0.0f, 1.0f)
    float frand_lt() {
      return rand() / float(rand_max() + 1);
    }

    /// Get a random floating point number in the range [0.0f, 1.0f]
    float frand_lte() {
      return rand() / float(rand_max());
    }

    /// Get a random integer in the range [0, mod)
    Sint32 rand_lt(const Sint32 &mod) {
      assert(mod <= rand_max() + 1);
      return Sint32(frand_lt() * mod);
    }

    /// Get a random integer in the range [0, mod]
    Sint32 rand_lte(const Sint32 &mod) {
      assert(mod <= rand_max());
      return Sint32(frand_lt() * (mod + 1));
    }

  private:
    Uint32 m_random_value;
  };

}

#endif
