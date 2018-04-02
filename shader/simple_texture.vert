#version 330 core

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform vec4 light;

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
	mat4 mvp = p * v * m;
    gl_Position = mvp * pos;
    out_uv = uv;
    out_normal = vec4(transpose(inverse(mat3(m))) * normal.xyz, 0.0f);
	out_color = color;
	light_direction = vec4( normalize(vec3(2.0f, -2.0f, 2.0f) - (m * pos).xyz),0.0f); // light - pos;
}