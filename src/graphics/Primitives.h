#pragma once

#include "Geometry.h"

namespace dgl
{
	namespace primitives
	{

		inline Geometry quad(float width = 1.0f, float height = 1.0f, const glm::vec4& color = glm::vec4{ 0,0,0,0 })
		{
			const auto w = 0.5f*width;
			const auto h = 0.5f*height;
			const auto normal = glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f };
			return {
				std::vector<GLuint>{ 0, 1, 2, 2, 3, 0 },
				std::vector<Vertex>{
					Vertex{ glm::vec4{ -w, -h, 0.0f, 1.0f }, normal, color, glm::vec2{ 0.0f, 0.0f } },
					Vertex{ glm::vec4{ w, -h, 0.0f, 1.0f }, normal, color, glm::vec2{ 1.0f, 0.0f } },
					Vertex{ glm::vec4{ w, h, 0.0f, 1.0f }, normal, color, glm::vec2{ 1.0f, 1.0f } },
					Vertex{ glm::vec4{ -w, h, 0.0f, 1.0f }, normal, color, glm::vec2{ 0.0f, 1.0f } }
				} };
		}

		/*
		  7-------6
		 /|      /|
		4-|-----5 |
		| 3-----|-2
		|/		|/
		0-------1
		*/

		inline Geometry cube(float side_length = 1.0f, const glm::vec4& color = glm::vec4{ 0,0,0,0 })
		{
			const auto w = side_length * 0.5f;
			const auto indices = std::vector<GLuint>{
				0,1,2,2,3,0,
				0,1,5,5,4,0,
				1,2,6,6,5,1,
				2,3,7,7,6,2,
				3,0,4,4,7,3,
				4,5,6,6,7,4
			};
			const auto vertices = std::vector<Vertex>{
				Vertex{ glm::vec4{-w,-w,-w, 1.0f}, glm::normalize(glm::vec4{-1.0f,-1.0f, -1.0f, 0.0f}), color, glm::vec2{0.0f, 0.0f}},
				Vertex{ glm::vec4{w,-w,-w, 1.0f }, glm::normalize(glm::vec4{ 1.0f,-1.0f, -1.0f, 0.0f }), color, glm::vec2{ 1.0f, 0.0f }},
				Vertex{ glm::vec4{w,w,-w, 1.0f }, glm::normalize(glm::vec4{ 1.0f,1.0f, -1.0f, 0.0f }), color, glm::vec2{ 1.0f, 1.0f }},
				Vertex{ glm::vec4{-w,w,-w, 1.0f }, glm::normalize(glm::vec4{ -1.0f,1.0f, -1.0f, 0.0f }), color, glm::vec2{ 0.0f, 1.0f } },
				Vertex{ glm::vec4{ -w,-w,w, 1.0f }, glm::normalize(glm::vec4{ -1.0f,-1.0f, 1.0f, 0.0f }), color, glm::vec2{ 0.0f, 0.0f } },
				Vertex{ glm::vec4{ w,-w,w, 1.0f }, glm::normalize(glm::vec4{ 1.0f,-1.0f, 1.0f, 0.0f }), color, glm::vec2{ 1.0f, 0.0f } },
				Vertex{ glm::vec4{ w,w,w, 1.0f }, glm::normalize(glm::vec4{ 1.0f,1.0f, 1.0f, 0.0f }), color, glm::vec2{ 1.0f, 1.0f } },
				Vertex{ glm::vec4{ -w,w,w, 1.0f }, glm::normalize(glm::vec4{ -1.0f,1.0f, 1.0f, 0.0f }), color, glm::vec2{ 0.0f, 1.0f } },
			};
			return { indices, vertices };
		}
	}
}