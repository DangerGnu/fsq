#pragma once

#include "graphics/SceneObj.h"

namespace dgl
{
	// A game level (just a background atm)
	struct Map
	{
		Map() {}
		Map(float width, float height, Texture texture, Shader shader);

		float width = 0;
		float height = 0;
		SceneObj scene_obj = {};
	};
}