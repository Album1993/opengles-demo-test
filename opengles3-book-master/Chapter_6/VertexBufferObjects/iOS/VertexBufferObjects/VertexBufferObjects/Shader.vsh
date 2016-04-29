#version 300 es
layout(location = 0)in vec4 a_position;
layout(location = 1)in vec4 a_color;
out vec4 v_color;
uniform float offset;
void main()
{
    v_color = a_color;
    gl_Position = a_position;
    gl_Position.x += offset;
}