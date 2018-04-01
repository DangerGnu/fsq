#include "Map.h"
#include "graphics/Primitives.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"	

using namespace dgl;

Map::Map(float width, float height, Texture texture, Shader shader) : width(width), height(height)
{
	scene_obj.tex = texture;
	scene_obj.batch = Batch{ shader, primitives::quad(width, height) };
	auto offset = glm::vec3{ 0 }; // glm::vec3{ -(float)width*0.5f, -(float)height*0.5f, 0.0f };
	scene_obj.model_mat = glm::translate(glm::mat4{ 1.0f }, offset);
}

