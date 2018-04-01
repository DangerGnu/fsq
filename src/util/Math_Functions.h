#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace dgl
{
	// Assuming 2D vectors in xy plane.
	inline glm::quat rotation_between_vectors_2d(const glm::vec3& start, const glm::vec3& dest)
	{
		float cos = dot(start, dest);
		glm::vec3 axis = glm::cross(start, dest);

		if (cos < -1.0f + std::numeric_limits<float>::epsilon())
		{
			// special case for opposite direction anomaly
			axis = glm::vec3{ 0.0f,0.0f,1.0f };
			return glm::angleAxis(glm::radians(180.0f), axis);
		}

		float s = sqrt((1.0f + cos)*2.0f);
		float s_inv = 1.0f / s;
		return glm::quat{
			s * 0.5f,
			axis.x * s_inv,
			axis.y * s_inv,
			axis.z * s_inv };
	}
}