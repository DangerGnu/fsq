#pragma once

#include "GL_Headers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>


namespace dgl
{
	class Camera
	{
	public:
		Camera() {};
		Camera(
			const glm::vec3& position,
			const glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
			const GLfloat fov_rad = glm::pi<GLfloat>() * 0.5f,
			const GLfloat aspect_ratio = 800.0f / 600.0f,
			const glm::vec3& up = glm::vec3{0.0f, 0.0f, 1.0f},
			const GLfloat near = 0.001f,
			const GLfloat far = 1000.0f) :
			m_position(position),
			m_target(target),
			m_fov_rad(fov_rad),
			m_aspect_ratio(aspect_ratio),
			m_up(up),
			m_near(near),
			m_far(far)
		{}
		
		glm::vec3 m_position;
		glm::vec3 m_target;
		GLfloat m_fov_rad;
		GLfloat m_aspect_ratio;
		glm::vec3 m_up;
		GLfloat m_near;
		GLfloat m_far;

		glm::mat4 view_projection() const {
			auto view = glm::lookAt(m_position, m_target, m_up);
			auto projection = glm::perspective(m_fov_rad, m_aspect_ratio, m_near, m_far);
			return projection * view;
		}
	};
}
