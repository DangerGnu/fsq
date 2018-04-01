#pragma once

#include "Shader.h"
#include "Geometry.h"

namespace dgl
{
	class Batch
	{
	public:
		Batch() {}
		Batch(Shader shader) : m_shader(shader) {}
		Batch(Shader shader, const Geometry& geometry); // caller still holds geometry
		Shader shader() const { return m_shader; }
		GLuint vao() const { return m_vao; }
		GLuint num() const { return m_num; } 
		void set_shader(Shader sh) { m_shader = sh; }
	private:
		Shader m_shader;
		GLuint m_vao = 0;
		GLuint m_num = 0; // number of vertices in the batch
		GLuint m_draw_type = GL_STATIC_DRAW;

		static GLuint build_vao(Shader shader, const Geometry& geometry, GLuint draw_type);
	};
}
