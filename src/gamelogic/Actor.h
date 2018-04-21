#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "graphics/SceneObj.h"
#include "util/Math_Functions.h"

namespace dgl
{
	// a moving thing within the game
	struct Actor {
		Actor() {};
		Actor(glm::vec3 position, SceneObj&& obj) :
			pos{ position }, scene_obj{ std::move(obj) } {}
		
		glm::vec3 pos = glm::vec3{ 0 };
		glm::vec3 color = glm::vec3{ 0.0, 0.0, 0.0 };
		SceneObj scene_obj = {};
	};
	// just use the model mat of the scene obj
	inline glm::mat4 model_matrix(const Actor& a)
	{
		//glm::mat4 orientation = glm::toMat4(dgl::rotation_between_vectors_2d(glm::vec3(1.0f, 0.0f, 0.0f), a.view));
		return glm::translate(glm::mat4{ 1.0f }, a.pos) * a.scene_obj.model_mat;
	}
	inline glm::mat2 texture_transform(const Actor& a)
	{
		// flip X according to view direction
		//float half_space_test = glm::dot(glm::vec3{ 1.0f, 0.0f, 0.0f }, a.view);
		//float scale = glm::sign(half_space_test);
		//return a.scene_obj.tex_transform * glm::mat2{ scale, 0.0f, 0.0f, 1.0f };
		return glm::mat2(1);
	}
}