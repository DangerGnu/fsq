#pragma once

#include "GL_Headers.h"

#include <vector>
#include <glm/glm.hpp>

namespace dgl
{
	struct Vertex	// can't really control padding - hope this works anyway
	{
		glm::vec4 vertex;
		glm::vec4 normal;
		glm::vec4 color; // todo remove color? ... naw.
		glm::vec2 uv;
	};
	struct Geometry
	{
		Geometry() : indices{}, vertices{} {}
		Geometry(std::vector<GLuint> indices, std::vector<Vertex> vertices) : indices{ indices }, vertices{ vertices } {}
		std::vector<GLuint> indices;
		std::vector<Vertex> vertices;
	};
	// all sorts of geometry functions here
}

