#version 330 core

uniform vec3 overlay_color;
uniform mat2 tex_transform;
uniform sampler2D tex;

in vec4 out_normal;
in vec4 out_color;
in vec2 out_uv;

in vec4 light_direction;

out vec4 color;

void main()
{
	float factor = (0.2 + max(dot(out_normal.xyz, light_direction.xyz),0.0));
	vec3 col = out_color.rgb * factor; // texture(tex, tex_transform * out_uv).abgr + out_color;
	color = vec4(col, 1.0);
}