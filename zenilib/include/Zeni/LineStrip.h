/**
 * \class Zeni::LineStrip<VERTEX>
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of a collection of lines
 *
 * \author voigtjr
 *
 * Contact: voigtjr@gmail.com
 */

#ifndef ZENI_LINESTRIP_H
#define ZENI_LINESTRIP_H

#include <Zeni/Color.h>
#include <Zeni/Renderable.h>
#include <Zeni/Vector3f.h>

namespace Zeni {

  class LineStrip : public Renderable {
  public:
    LineStrip(const float* verts, int size, int length, const Vector3f pos = kZero3, const Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f) );
	~LineStrip() {}

    LineStrip(const LineStrip &rhs);
    LineStrip & operator=(const LineStrip &rhs);

    inline const float* const get_verts() const; ///< Gets the pointer to the vert array
    inline void set_verts(const float* verts);   ///< Set the location of the vertex array allocated outside this class

    inline const int& get_length() const;        ///< The number of verts in the array
    inline void set_length(const int& length);   ///< Provide the number of vers in the array

	// @todo get_position should return a vector... none of this Point nonsense
    inline Point3f get_position() const;     ///< Get the rendering offset
    inline void set_position(const Vector3f &vector); ///< Set an offset from which to render the strip.

    inline const Vector3f& get_scale() const;			
    inline void set_scale(const Vector3f &vector);

	inline const int& get_size() const;   ///< The number of floats that make up a vert in the external vert array.

	inline const Color& get_color() const;
    inline void set_color(const Color& color);

    inline void render() const;

	virtual bool is_3d() const;
#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif


    LineStrip * get_duplicate() const; ///< Get a duplicate of the LineStrip

  private:
    const float* m_Verts;
	int m_Size;
	int m_Length;
	Vector3f m_Position;
	Vector3f m_Scale;

	Color m_Color;
  };

// VERTEX template version
  template <typename VERTEX>
  class LineStripT : public Renderable {
  public:
    LineStripT(const VERTEX* verts, int length, const Vector3f pos = kZero3, const Vector3f scale = kOne3 );
	~LineStripT() {}

	// Do not implement!
    LineStripT(const LineStrip &rhs);
    LineStripT & operator=(const LineStrip &rhs);

    inline const VERTEX* const get_verts() const; ///< Gets the pointer to the vert array
    inline void set_verts(const VERTEX* verts);   ///< Set the location of the vertex array allocated outside this class

    inline const int& get_length() const;        ///< The number of verts in the array
    inline void set_length(int length);   ///< Provide the number of vers in the array

	inline void set_stride(int stride);	///< Use to override the default space between verticies

	// @todo get_position should return a vector... none of this Point nonsense
    inline Point3f get_position() const;     ///< Get the rendering offset
    inline void set_position(const Vector3f &vector); ///< Set an offset from which to render the strip.

    inline const Vector3f& get_scale() const;			
    inline void set_scale(const Vector3f &vector);

    inline void render() const;

	virtual bool is_3d() const;
#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
#endif
#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

    LineStripT * get_duplicate() const; ///< Get a duplicate of the LineStripT

	unsigned int m_HACKtype; ///< defaults to GL_LINE_STRIP
  private:
    const VERTEX* m_Verts;
	int m_Length;
	int m_Stride;			///< defaults to sizeof(VERTEX), use set_stride to override.
	Vector3f m_Position;
	Vector3f m_Scale;
  };

} // namespace Zeni

#endif
