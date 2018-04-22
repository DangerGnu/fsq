#pragma once

#include "GL_Headers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

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
			const GLfloat near = 0.1f,
			const GLfloat far = 1000.0f) :
			m_position(position),
			m_target(target),
			m_fov_rad(fov_rad),
			m_aspect_ratio(aspect_ratio),
			m_up(up),
			m_near(near),
			m_far(far)
		{}
		glm::quat m_orientation = glm::quat{ 1,0,0,0 };
		glm::vec3 m_position;
		glm::vec3 m_target;
		GLfloat m_fov_rad;
		GLfloat m_aspect_ratio;
		glm::vec3 m_up;
		GLfloat m_near;
		GLfloat m_far;

		glm::mat4 view() const {
			auto up = glm::vec3(m_orientation * glm::vec4(0, 0, 1, 0));
			return glm::lookAt(m_position, m_target, up);
		}

		glm::mat4 projection() const {
			return glm::perspective(m_fov_rad, m_aspect_ratio, m_near, m_far);
		}

		glm::mat4 view_projection() const {
			auto view = glm::lookAt(m_position, m_target, m_up);
			auto projection = glm::perspective(m_fov_rad, m_aspect_ratio, m_near, m_far);
			return projection * view;
		}
	};
}
