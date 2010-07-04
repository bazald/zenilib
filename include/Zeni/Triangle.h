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

#ifndef ZENI_TRIANGLE_H
#define ZENI_TRIANGLE_H

#include <Zeni/Renderable.h>

namespace Zeni {

  /**
   * \ingroup zenilib
   *
   * \brief An Abstraction of a Triangle
   *
   * \author bazald
   *
   * Contact: bazald@zenipex.com
   */

  template <typename VERTEX>
  class Triangle : public Renderable {
  public:
    Triangle();
    inline Triangle(const VERTEX &vertex0,
                    const VERTEX &vertex1,
                    const VERTEX &vertex2);

    Triangle(const Triangle<VERTEX> &rhs);
    Triangle<VERTEX> & operator=(const Triangle<VERTEX> &rhs);

    /// Tell the rendering system if we're using 3D coordinates
    virtual bool is_3d() const;

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

    Triangle<VERTEX> * get_duplicate() const; ///< Get a duplicate of the Triangle

    Triangle<VERTEX> * get_duplicate_subt0() const; ///< Get quarter 0 of the Triangle; Can be used for software LOD increase
    Triangle<VERTEX> * get_duplicate_subt1() const; ///< Get quarter 1 of the Triangle; Can be used for software LOD increase
    Triangle<VERTEX> * get_duplicate_subt2() const; ///< Get quarter 2 of the Triangle; Can be used for software LOD increase
    Triangle<VERTEX> * get_duplicate_subt3() const; ///< Get quarter 3 of the Triangle; Can be used for software LOD increase

    // Indexing
    inline const VERTEX & operator[](const int &index) const; ///< Get 'index'
    inline VERTEX & operator[](const int &index); ///< Get 'index'

    VERTEX a;
    VERTEX b;
    VERTEX c;

  private:
    void * m_alignment_rubbish;
  };

}

#endif
