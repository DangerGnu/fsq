#pragma once

#include "Primitives.h"
#include "GL_Headers.h"
#include "SceneObj.h"
#include <algorithm>

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

		SceneObj cube_grid(const float cube_size, int x, int y, int z, float grid_spacing, Shader shader, const glm::vec4& color = glm::vec4(1) )
		{
			Geometry geo;
			geo.vertices.reserve(x*y*z * 8);
			geo.indices.reserve(x*y*z * 36);
			auto index_offset = 0;
			for (int i = 0; i < x; ++i)
			{
				for (int j = 0; j < y; ++j)
				{
					for (int k = 0; k < z; ++k)
					{
						auto geo_temp = primitives::cube(cube_size, glm::vec4(1));
						std::transform(begin(geo_temp.vertices), end(geo_temp.vertices), back_inserter(geo.vertices), [=](const auto& v) {
							auto ret = v;
							ret.vertex += glm::vec4(grid_spacing * i - grid_spacing * x / 2, grid_spacing * j - grid_spacing * y / 2, grid_spacing * k - grid_spacing * z / 2, 0);
							return ret;
						});
						std::transform(begin(geo_temp.indices), end(geo_temp.indices), back_inserter(geo.indices), [=](const auto& index) {
							return index + index_offset;
						});
						index_offset += 8;
					}
				}
			}
			auto batch = Batch{ shader, geo };
			return SceneObj{ {}, batch, color };
		}
	}
}