uniform mat4 modelViewProj;
attribute vec4 a_position;
attribute vec4 a_color;
varying vec4 v_color;

void main()
{
     gl_Position = modelViewProj * vec4(a_position.x + 100.0, a_position.y + 100.0, a_position.z, a_position.w);
     v_color = a_color;
}
