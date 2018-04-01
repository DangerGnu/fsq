#include "Batch.h"

using namespace dgl;

GLuint dgl::Batch::build_vao(Shader shader, const Geometry& geo, GLuint draw_type)
{
	// generate 1 vao (you could do more but that's kind of overcomplicating things here)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// create the index buffer
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, geo.indices.size() * sizeof(GLuint),
		&geo.indices.front(), draw_type);
	// create the vertex buffer with interleaved data
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER, geo.vertices.size() * sizeof(dgl::Vertex),
		&geo.vertices.front().vertex.x, draw_type);
	// send data to shader
	GLuint pos_loc = glGetAttribLocation(shader.handle(), "pos");
	glBindAttribLocation(shader.handle(), pos_loc, "pos");
	glVertexAttribPointer(pos_loc, 4, GL_FLOAT, GL_FALSE, sizeof(dgl::Vertex), (void*)0);
	glEnableVertexAttribArray(pos_loc);

	GLuint norm_loc = glGetAttribLocation(shader.handle(), "normal");
	glBindAttribLocation(shader.handle(), norm_loc, "normal");
	glVertexAttribPointer(norm_loc, 4, GL_FLOAT, GL_FALSE, sizeof(dgl::Vertex), (void*)16);
	glEnableVertexAttribArray(norm_loc);

	GLuint color_loc = glGetAttribLocation(shader.handle(), "color");
	glBindAttribLocation(shader.handle(), color_loc, "color");
	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(dgl::Vertex), (void*)32);
	glEnableVertexAttribArray(color_loc);

	GLuint uv_loc = glGetAttribLocation(shader.handle(), "uv");
	glBindAttribLocation(shader.handle(), uv_loc, "uv");
	glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, sizeof(dgl::Vertex), (void*)48);
	glEnableVertexAttribArray(uv_loc);
		
	glBindVertexArray(0);
	// create 
	// generate a vao
	// put all the geometry into a single vbo
	// return the vao
	return vao;
}

dgl::Batch::Batch(Shader shader, const Geometry& geometry) : m_shader(shader)
{
	m_vao = build_vao(m_shader, geometry, m_draw_type);
	m_num = (GLuint)geometry.indices.size();
}
