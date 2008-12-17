/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
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
 * \class Zeni::Sounds
 *
 * \ingroup Zenilib
 *
 * \brief A Sound_Buffer Database Singleton
 *
 * The Sounds Singleton stores Sound_Buffers to be played from Sound_Sources.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUND_SOURCE_POOL_H
#define ZENI_SOUND_SOURCE_POOL_H

#include <Zeni/Sound_Source.h>

#include <list>

namespace Zeni {

  class Sound_Source_Pool {
    // Get reference to only instance;
    friend Sound_Source_Pool & get_Sound_Source_Pool(); ///< Get access to the singleton.

    friend class Sound_Source;

    Sound_Source_Pool();
    ~Sound_Source_Pool();

    // Undefined
    Sound_Source_Pool(const Sound_Source_Pool &);
    Sound_Source_Pool & operator=(const Sound_Source_Pool &);

  public:
    class Replacement_Policy {
    public:
      /// (Default) Priority Sort:  Playing/Not-Playing, Priority
      virtual bool operator()(const Sound_Source &lhs, const Sound_Source &rhs) const;

      bool operator()(const Sound_Source * const &lhs, const Sound_Source * const &rhs) const;
    };

    class Positional_Replacement_Policy : public Replacement_Policy {
    public:
      Positional_Replacement_Policy(const Point3f &listener_position_);

      /// Positional Priority Sort:  Playing/Not-Playing, Priority, Computed Gain
      virtual bool operator()(const Sound_Source &lhs, const Sound_Source &rhs) const;

      Point3f listener_position;
    };

    const Replacement_Policy & get_Replacement_Policy() const; ///< Get the Replacement_Policy
    void give_Replacement_Policy(Replacement_Policy * const &replacement_policy); ///< Give the Sound_Source_Pool a new Replacement_Policy (which it will delete later)
    void lend_Replacement_Policy(Replacement_Policy * const &replacement_policy); ///< Give the Sound_Source_Pool a new Replacement_Policy (which it will NEVER delete)

    void pause_all(); ///< Pause all Sound_Sources.
    void unpause_all(); ///< Unpause all paused Sound_Sources.
    void purge(); ///< Purge all Sound_Source_HW

    void update(); ///< Redistribute hardware Sound_Sources according to the Replacement_Policy.  Newer Sound_Sources are implicitly prioritized over older Sound_Sources.  (Called automatically)

  private:
    void set_Replacement_Policy(Replacement_Policy * const &replacement_policy); ///< Set the Replacement_Policy directly
    void insert_Sound_Source(Sound_Source &sound_source); // on Sound_Source construction
    void remove_Sound_Source(Sound_Source &sound_source); // on Sound_Source destruction

    std::set<Sound_Source_HW *> m_assigned_hw;
    std::vector<Sound_Source *> m_handles;

    Replacement_Policy * m_replacement_policy;
    bool delete_m_replacement_policy;
  };

  Sound_Source_Pool & get_Sound_Source_Pool(); ///< Get access to the singleton.

}

#endif
