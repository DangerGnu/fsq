#pragma once

#include "Primitives.h"
#include "GL_Headers.h"
#include "SceneObj.h"

namespace dgl
{
	namespace scene_primitives
	{
		inline SceneObj textured_quad(const float width, const float height, Texture texture, Shader shader)
		{
			auto geo = primitives::quad(width, height, { 0.0,0.0,0.0,0.0 });
			auto batch = Batch{ shader, geo };
			return SceneObj{ texture, batch };
		}

		inline SceneObj colored_cube(const float side_length, const glm::vec4& color, Texture texture, Shader shader)
		{
			auto geo = primitives::cube(side_length, color);
			auto batch = Batch{ shader, geo };
			return SceneObj{ {}, batch, color };
		}
	}
}