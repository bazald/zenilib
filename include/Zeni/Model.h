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
 * \class Zeni::Model
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of a 3D Model
 *
 * The Model class is responsible for loading 3ds models into a Vertex_Buffer
 * using lib3ds.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Model_Visitor
 *
 * \ingroup Zenilib
 *
 * \brief A visitor base class
 *
 * A Model_Visitor is called once per node each time visit_nodes
 * is called.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Model_Extents
 *
 * \ingroup Zenilib
 *
 * \brief A visitor for determining the extents or bounds of a model
 *
 * \note You should create a new Model_Extents for each call of visit_nodes.
 *
 * \warning It gives valid results for the first frame of an animation only.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_MODEL_H
#define ZENI_MODEL_H

#include <Zeni/Coordinate.h>
#include <Zeni/Render_Wrapper.h>
#include <Zeni/Thread.h>
#include <Zeni/Vector3f.h>

#include <lib3ds/file.h>
#include <lib3ds/node.h>
#include <memory>

namespace Zeni {

  class Model;
  struct Quaternion;

  class Model_Visitor {
  public:
    Model_Visitor() {}
    virtual ~Model_Visitor() {}

    // Note visiting function
    virtual void operator()(const Model & /*model*/, Lib3dsNode * const & /*node*/) {}

    // Mesh visiting function
    virtual void operator()(const Model & /*model*/, Lib3dsNode * const & /*node*/, Lib3dsMesh * const & /*mesh*/) {}
  };

  class Model_Extents : public Model_Visitor {
  public:
    Model_Extents();

    virtual void operator()(const Model &model, Lib3dsNode * const &node, Lib3dsMesh * const &mesh);

    Point3f lower_bound, upper_bound; ///< The bounding box of model, first frame only if animated
    bool started;
  };

  class Model {
  public:
    /// The only way to create a Model
    Model(const std::string &filename);
    Model(const Model &rhs);
    ~Model();

    Model & operator =(const Model &rhs);

    // Accessors
    inline Lib3dsFile * const & get_file() const; ///< Get the full 3ds file info
    virtual Point3f get_position() const; ///< Get the position of the Model
    inline const Model_Extents & get_extents() const; ///< Get the extents of the Model
    inline float get_keyframes() const; ///< Get the number of keyframes; may be higher than you expect
    inline const Vector3f & get_scale(); ///< Get the Model scale
    inline std::pair<Vector3f, float> get_rotate(); ///< Get the Model rotation
    inline const Point3f & get_translate(); ///< Get the Model translation
    inline const float & get_keyframe(); ///< Get the current (key)frame

    // Modifiers
    inline void set_scale(const Vector3f &multiplier); ///< Scale the Model
    inline void set_rotate(const float &angle, const Vector3f &ray); ///< Rotate the Model
    inline void set_rotate(const Quaternion &quaternion); ///< Rotate the Model
    inline void set_translate(const Point3f &vector); ///< Translate the Model
    inline void set_keyframe(const float &keyframe); ///< Set the current (key)frame; interpolation is automatic

    // Post-Order Traversal
    void visit_nodes(Model_Visitor &mv, Lib3dsNode *node = 0) const; ///< Visit all nodes
    void visit_meshes(Model_Visitor &mv, Lib3dsNode *node = 0, Lib3dsMesh *mesh = 0) const; ///< Visit all meshes

    void render() const;

    // Thread-Unsafe versions
    inline Lib3dsFile * const & thun_get_file() const; ///< Get the full 3ds file info - Thread Unsafe Version

  private:
    std::string m_filename;
    Lib3dsFile *m_file;
    float m_keyframe;

    mutable Model_Visitor *m_unrenderer;

    Model_Extents m_extents;
    Point3f m_position;

    Vector3f m_scale, m_rotate;
    Point3f m_translate;
    float m_rotate_angle;
    
    class Loader : public Task {
      Loader(const Loader &);
      Loader & operator=(const Loader &);
      
    public:
      Loader(Model &model) : m_model(model) {}
      
      int function();
      
    private:
      Model &m_model;
    };
    
    void load();
    
    mutable Loader m_loader;
    mutable Runonce_Computation m_loader_op;
  };

  struct Model_Init_Failure : public Error {
    Model_Init_Failure() : Error("Zeni Model Failed to Initialize Correctly") {}
  };

  struct Model_Render_Failure : public Error {
    Model_Render_Failure() : Error("Zeni Model Failed to Render") {}
  };

}

#endif

#ifdef ZENI_INLINES
#include <Zeni/Model.hxx>
#endif
