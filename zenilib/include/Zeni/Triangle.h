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
    Triangle(const VERTEX &vertex0,
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
    const VERTEX & operator[](const int &index) const; ///< Get 'index'
    VERTEX & operator[](const int &index); ///< Get 'index'

    VERTEX a;
    VERTEX b;
    VERTEX c;

  private:
    void * m_alignment_rubbish;
  };

}

#endif
