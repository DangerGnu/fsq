#pragma once

#include<vector>
#include "graphics/GL_Headers.h"
#include "graphics/Batch.h"
#include "gamelogic/Actor.h"
#include "gamelogic/Map.h"

#include "glm/glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace dgl
{
	inline void draw(const Batch& batch)
	{
		// bind the vao
		glBindVertexArray(batch.vao());
		// draw the array
		glDrawElements(GL_TRIANGLES, batch.num(), GL_UNSIGNED_INT, (void*)0);
		// unbind the vao
		glBindVertexArray(0);
	}
	// setting the uniforms of the shader should not be the responsibility of the draw function. Read into UBOs
	inline void draw(const Actor& a, const glm::mat4& v, const glm::mat4& p)
	{
		auto tex_mat = texture_transform(a);
		a.scene_obj.tex.bind(GL_TEXTURE0);
		auto& shader = a.scene_obj.batch.shader();
		glUseProgram(shader.handle());
		shader.uniform("overlay_color", a.color);
		shader.uniform("m", model_matrix(a));
		shader.uniform("v", v);
		shader.uniform("p", p);
		shader.uniform("tex_transform", tex_mat);
		draw(a.scene_obj.batch);
		// glUseProgram(0); don't really need that
	}
	inline void draw(const Map& map, const glm::mat4& vp)
	{
		auto mvp = vp * map.scene_obj.model_mat;
		map.scene_obj.tex.bind(GL_TEXTURE0);
		auto& shader = map.scene_obj.batch.shader();
		shader.uniform("mvp", mvp);
		shader.uniform("tex_transform", map.scene_obj.tex_transform);
		draw(map.scene_obj.batch);
	}
}