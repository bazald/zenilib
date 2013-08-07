/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2013 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>

#include <d3dx9.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

GLfloat vertices[12] = {  0.0f,   0.0f,
                          0.0f, 300.0f,
                        400.0f,   0.0f};
GLfloat colors[12] = {1.0f, 0.0f, 0.0f, 1.0f,
                      0.0f, 1.0f, 0.0f, 1.0f,
                      0.0f, 0.0f, 1.0f, 1.0f};
GLushort indices[3] = {0, 1, 2};

class Modern_State : public Gamestate_Base {
  Modern_State(const Modern_State &);
  Modern_State operator=(const Modern_State &);

public:
  Modern_State()
    : m_vertex_shader(get_Video().create_Vertex_Shader("shaders/shader.vert")),
    m_fragment_shader(get_Video().create_Fragment_Shader("shaders/shader.frag")),
    m_program(get_Video().create_Program()),
    a_position(0),
    a_color(0),
    vertexDecl(0),
    vertexBuffer(0),
    colorBuffer(0),
    indexBuffer(0)
  {
    set_pausable(true);
    
    m_program->attach(*m_vertex_shader);
    m_program->attach(*m_fragment_shader);
    m_program->link();

    memset(buffers, 0, sizeof(buffers));

    if(Program_GL_Fixed * program = dynamic_cast<Program_GL_Fixed *>(m_program.get())) {
      glGenBuffersARB(3, buffers);

      glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers[0]);
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STATIC_DRAW_ARB);
      a_position = glGetAttribLocationARB(program->get(), "a_position");

      glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers[1]);
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(colors), colors, GL_STATIC_DRAW_ARB);
      a_color = glGetAttribLocationARB(program->get(), "a_color");

      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, buffers[2]);
      glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(indices), indices, GL_STATIC_DRAW_ARB);

      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    }
    else if(Program_GL_Shader * program = dynamic_cast<Program_GL_Shader *>(m_program.get())) {
      glGenBuffers(3, buffers);

      glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      a_position = glGetAttribLocation(program->get(), "a_position");

      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
      a_color = glGetAttribLocation(program->get(), "a_color");

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else if(Program_DX9 * program = dynamic_cast<Program_DX9 *>(m_program.get())) {
      Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());

      D3DVERTEXELEMENT9 decl[] = {{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                                  {1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                                  D3DDECL_END()};
      vdx.get_d3d_device()->CreateVertexDeclaration(decl, &vertexDecl);
      
      void * data;
      vdx.get_d3d_device()->CreateVertexBuffer(sizeof(vertices), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vertexBuffer, 0);
      if(FAILED(vertexBuffer->Lock(0, 0, &data, 0)))
        throw Error("Lock failed.");
      memcpy(data, vertices, sizeof(vertices));
      vertexBuffer->Unlock();
        
      vdx.get_d3d_device()->CreateVertexBuffer(sizeof(colors), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &colorBuffer, 0);
      if(FAILED(colorBuffer->Lock(0, 0, &data, 0)))
        throw Error("Lock failed.");
      memcpy(data, colors, sizeof(colors));
      colorBuffer->Unlock();

      vdx.get_d3d_device()->CreateIndexBuffer(sizeof(indices), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuffer, 0);
      if(FAILED(indexBuffer->Lock(0, 0, &data, 0)))
        throw Error("Lock failed.");
      memcpy(data, indices, sizeof(indices));
      indexBuffer->Unlock();
    }
  }

  ~Modern_State() {
    if(dynamic_cast<Video_DX9 *>(&get_Video())) {
      if(indexBuffer)
        indexBuffer->Release();
      if(colorBuffer)
        colorBuffer->Release();
      if(vertexBuffer)
        vertexBuffer->Release();
      if(vertexDecl)
        vertexDecl->Release();
    }
    else
      glDeleteBuffers(3, buffers);
  }

private:
  void render() {
    Video &vr = get_Video();
    vr.set_backface_culling(false);
    
    const Matrix4f modelViewMatrix = Matrix4f::Orthographic(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    const bool use_shaders = get_Game().get_key_state(SDLK_SPACE);

    if(use_shaders) {
      vr.set_program(*m_program);

      if(Program_GL_Fixed * program = dynamic_cast<Program_GL_Fixed *>(m_program.get())) {
        GLuint modelViewProj = glGetUniformLocationARB(program->get(), "modelViewProj");
        glUniformMatrix4fvARB(modelViewProj, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&modelViewMatrix));

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers[0]);
        glVertexAttribPointerARB(a_position, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArrayARB(a_position);
        
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers[1]);
        glVertexAttribPointerARB(a_color, 4, GL_FLOAT, GL_TRUE, 0, 0);
        glEnableVertexAttribArrayARB(a_color);
        
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, buffers[2]);
        glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArrayARB(a_position);
        glDisableVertexAttribArrayARB(a_color);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
      }
      else if(Program_GL_Shader * program = dynamic_cast<Program_GL_Shader *>(m_program.get())) {
        GLuint modelViewProj = glGetUniformLocation(program->get(), "modelViewProj");
        glUniformMatrix4fv(modelViewProj, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&modelViewMatrix));

        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(a_position);
        
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glVertexAttribPointer(a_color, 4, GL_FLOAT, GL_TRUE, 0, 0);
        glEnableVertexAttribArray(a_color);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
        glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(a_position);
        glDisableVertexAttribArray(a_color);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
      }
      else if(Program_DX9 * program = dynamic_cast<Program_DX9 *>(m_program.get())) {
        Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());
        Shader_DX9 &vsdx = dynamic_cast<Shader_DX9 &>(*m_vertex_shader.get());

        if(FAILED(vsdx.get_constant_table()->SetMatrix(vdx.get_d3d_device(), "$_modelViewProj", reinterpret_cast<const D3DXMATRIX *>(&modelViewMatrix))))
          throw Error("Setting modelViewProj failed.");
      }
    }
    else {
      vr.set_view_matrix(modelViewMatrix);
      vr.set_projection_matrix(Matrix4f::Identity());
    }

    if(Program_DX9 * program = dynamic_cast<Program_DX9 *>(m_program.get())) {
      Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());

      vdx.get_d3d_device()->SetVertexDeclaration(vertexDecl);
      vdx.get_d3d_device()->SetStreamSource(0, vertexBuffer, 0, 2 * sizeof(float));
      vdx.get_d3d_device()->SetStreamSource(1, colorBuffer, 0, 4 * sizeof(float));
      vdx.get_d3d_device()->SetIndices(indexBuffer);

      vdx.get_d3d_device()->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 3, 0, 1);
        
      vdx.get_d3d_device()->SetIndices(0);
      vdx.get_d3d_device()->SetStreamSource(0, 0, 0, 0);
      vdx.get_d3d_device()->SetStreamSource(1, 0, 0, 0);
      vdx.get_d3d_device()->SetVertexDeclaration(0);
    }
    else if(!use_shaders) {
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(2, GL_FLOAT, 0, vertices);
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_FLOAT, 0, colors);
      
      glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_SHORT, indices);
      
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
    }

    if(use_shaders)
      vr.unset_program();
  }
  
  shared_ptr<Shader> m_vertex_shader;
  shared_ptr<Shader> m_fragment_shader;
  shared_ptr<Program> m_program;
  GLuint a_position;
  GLuint a_color;

  GLuint buffers[3];
  LPDIRECT3DVERTEXDECLARATION9 vertexDecl;
  LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
  LPDIRECT3DVERTEXBUFFER9 colorBuffer;
  LPDIRECT3DINDEXBUFFER9 indexBuffer;
};
