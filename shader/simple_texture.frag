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
	vec4 tex_col = texture(tex, out_uv).abgr; 
	vec3 col =  tex_col.rgb * factor + overlay_color * factor * 0.5f;
	color = vec4(col, tex_col.a);
}