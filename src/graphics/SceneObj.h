#pragma once

#include "Texture.h"
#include "Batch.h"

namespace dgl
{
	struct SceneObj
	{
		SceneObj() = default;
		SceneObj(Texture tex, Batch batch, glm::mat4 model_matrix = glm::mat4{ 1.0f }) : 
			tex(tex), batch(batch), model_mat(model_matrix), color(glm::vec4(1,1,1,1)) {}
		SceneObj(Texture tex, Batch batch, const glm::vec4& color, const glm::mat4& model_matrix = glm::mat4{ 1.0f }) :
			tex(tex), batch(batch), color(color), model_mat(model_matrix) {}
		Texture tex = {};
		Batch batch = {};
		glm::vec4 color = glm::vec4{ 0.0f,0.0f,0.0f,0.0f };
		glm::mat4 model_mat = glm::mat4{ 1.0f };
		glm::mat2 tex_transform = glm::mat2 { 1.0f };
	};
}