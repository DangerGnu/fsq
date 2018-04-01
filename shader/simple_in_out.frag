#version 330 core

uniform mat2 tex_transform;
uniform sampler2D tex;

in vec4 out_normal;
in vec4 out_color;
in vec2 out_uv;

in vec4 light_direction;

out vec4 color;

void main()
{
    color = out_color; //texture(tex, tex_transform * out_uv).abgr + out_color;
}