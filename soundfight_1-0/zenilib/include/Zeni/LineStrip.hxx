#ifndef ZENI_LINESTRIP_HXX
#define ZENI_LINESTRIP_HXX

// HXXed below
#include <Zeni/Video_DX9.h>
#include <Zeni/Video_GL.h>

#include <Zeni/LineStrip.h>

// Not HXXed
#include <Zeni/Vector3f.h>
#include <GL/glew.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <memory>

namespace Zeni {

  // plain old floats version
  inline LineStrip::LineStrip(const float* verts, int size, int length, const Vector3f pos, const Vector3f scale )
    : m_Verts(verts)
	, m_Size(size)
	, m_Length(length)
	, m_Position(pos)
	, m_Scale(scale)
	, m_Color( Zeni::Color( 1.0f, 1.0f, 0.0f, 1.0f ) )
  {
  }

  inline LineStrip::LineStrip(const LineStrip &rhs)
  {
	  *this = rhs;
  }
  inline LineStrip & LineStrip::operator=(const LineStrip &rhs)
  {
	  m_Verts = rhs.m_Verts;
	  m_Size = rhs.m_Size;
	  m_Scale = rhs.m_Scale;
	  m_Position = rhs.m_Position;
	  m_Scale = rhs.m_Scale;
	  m_Color = rhs.m_Color;
	  return *this;
  }

  inline const float * const LineStrip::get_verts() const {
    return m_Verts;
  }

  inline void LineStrip::set_verts(const float* verts) {
	  m_Verts = verts;
  }

  inline const int& LineStrip::get_length() const {
    return m_Length;
  }

  inline void LineStrip::set_length(const int& length) {
	  m_Length = length;
  }

  inline const int& LineStrip::get_size() const {
	  return m_Size;
  }

  inline const Color& LineStrip::get_color() const {
	  return m_Color;
  }

  inline void LineStrip::set_color(const Color& color) {
	  m_Color = color;
  }

  inline Point3f LineStrip::get_position() const
  {
	  return m_Position;
  }

  inline void LineStrip::set_position(const Vector3f &vector)
  {
	  m_Position = vector;
  }

  inline const Vector3f& LineStrip::get_scale() const
  {
	  return m_Scale;
  }

  inline void LineStrip::set_scale(const Vector3f &vector)
  {
	  m_Scale = vector;
  }

  inline void LineStrip::render() const {
	  Video &vr = get_Video();

	  vr.push_world_stack();

	  vr.translate_scene( m_Position );
	  //vr.rotate_scene(m_rotate, m_rotate_angle);
	  vr.scale_scene(m_Scale);

	  vr.render( *this );

	  vr.pop_world_stack();
  }

  inline bool LineStrip::is_3d() const {
	  return false;
  }

#ifndef DISABLE_GL
  inline void LineStrip::render_to(Video_GL &screen) const {

    // unbind any VBO's that might have been bound previously
    screen.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4fv( &m_Color[0] );
	glVertexPointer( m_Size, GL_FLOAT, 0, m_Verts );
	glDrawArrays( GL_LINE_STRIP, 0, m_Length );

    glDisableClientState(GL_VERTEX_ARRAY);
  }
#endif

#ifndef DISABLE_DX9
  inline void LineStrip::render_to(Video_DX9 &screen) const {
	// @TODO: DirectX implementation
	//assert( false );

	//screen.get_d3d_device()->SetFVF( D3DFVF_XYZ );
	screen.set_color( m_Color.get_argb() );

    //@todo need to pass color somehow
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_LINESTRIP, m_Length, m_Verts, m_Size);

	//screen.set_fvf();
  }
#endif

  inline LineStrip * LineStrip::get_duplicate() const {
    return new LineStrip(m_Verts, m_Length, m_Size, m_Position, m_Scale );
  }

// VERTEX template version
  template <typename VERTEX>
  LineStripT<VERTEX>::LineStripT(const VERTEX* verts, int length, const Vector3f pos, const Vector3f scale )
    : m_HACKtype( GL_LINE_STRIP )
    , m_Verts(verts)
	, m_Length(length)
	, m_Stride(sizeof(VERTEX))
	, m_Position(pos)
	, m_Scale(scale)
  {
  }

  template <typename VERTEX>
  const VERTEX * const LineStripT<VERTEX>::get_verts() const {
    return m_Verts;
  }

  template <typename VERTEX>
  void LineStripT<VERTEX>::set_verts(const VERTEX* verts) {
	  m_Verts = verts;
  }

  template <typename VERTEX>
  const int& LineStripT<VERTEX>::get_length() const {
    return m_Length;
  }

  template <typename VERTEX>
  void LineStripT<VERTEX>::set_length(int length) {
	  m_Length = length;
  }

  template <typename VERTEX>
  void LineStripT<VERTEX>::set_stride(int stride) {
	  m_Stride = stride;
  }

  template <typename VERTEX>
  Point3f LineStripT<VERTEX>::get_position() const
  {
	  return m_Position;
  }

  template <typename VERTEX>
  void LineStripT<VERTEX>::set_position(const Vector3f &vector)
  {
	  m_Position = vector;
  }

  template <typename VERTEX>
  const Vector3f& LineStripT<VERTEX>::get_scale() const
  {
	  return m_Scale;
  }

  template <typename VERTEX>
  void LineStripT<VERTEX>::set_scale(const Vector3f &vector)
  {
	  m_Scale = vector;
  }

  template <typename VERTEX>
  void LineStripT<VERTEX>::render() const {

    Video &vr = get_Video();

    vr.push_world_stack();

    vr.translate_scene( m_Position );
    //vr.rotate_scene(m_rotate, m_rotate_angle);
    vr.scale_scene(m_Scale);

	vr.render( *this );

    vr.pop_world_stack();  
  }

  template <typename VERTEX>
  bool LineStripT<VERTEX>::is_3d() const {
	if( m_Length > 0 )
	{
		return m_Verts[0].is_3d();
	}
	return false;
  }

#ifndef DISABLE_GL
  template <typename VERTEX>
  void LineStripT<VERTEX>::render_to(Video_GL &screen) const {

    // unbind any VBO's that might have been bound previously
    screen.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY); //@todo this assumes it has a color array :(

	const int vertSize = sizeof( m_Verts[0].position ) / sizeof( float ); 
	//@todo this assumes it's a float, but that's probably okay?
	glVertexPointer( vertSize, GL_FLOAT, m_Stride, &m_Verts[0].position );
	glColorPointer(4, GL_UNSIGNED_BYTE, m_Stride, &m_Verts[0].m_argb); //@todo this assumes it has a color array :(
	glDrawArrays( m_HACKtype, 0, m_Length );

	glDisableClientState(GL_COLOR_ARRAY); //@todo this assumes it has a color array :(
    glDisableClientState(GL_VERTEX_ARRAY);
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void LineStripT<VERTEX>::render_to(Video_DX9 &screen) const {
	// @TODO: DirectX implementation
	//assert( false );

	//screen.get_d3d_device()->SetFVF( D3DFVF_XYZ );

    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_LINESTRIP, m_Length, &m_Verts[0].position, m_Stride );

	//screen.set_fvf();
  }
#endif

  template <typename VERTEX>
  LineStripT<VERTEX> * LineStripT<VERTEX>::get_duplicate() const {
    return new LineStripT<VERTEX>(m_Verts, m_Length, m_Position, m_Scale );
  }

} // namespace Zeni


#include <Zeni/Video_DX9.hxx>
#include <Zeni/Video_GL.hxx>

#endif
