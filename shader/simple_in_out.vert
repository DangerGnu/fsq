#version 330 core

uniform mat4 mvp;

in vec4 pos;
in vec4 normal;
in vec4 color;
in vec2 uv;

out vec4 out_normal;
out vec4 out_color;
out vec2 out_uv;

out vec4 light_direction;

void main()
{
    gl_Position = mvp * pos;
    out_uv = uv;
    out_normal = normal;
	out_color = color;
}