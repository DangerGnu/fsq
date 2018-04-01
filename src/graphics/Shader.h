#pragma once
#include "GL_Headers.h"
#include <string>
#include <iostream>
#include "glm/glm.hpp"

// Normal Glsl Shader Program
// Extend this with access to variables as you go.
namespace dgl
{
	class Shader
	{
	public:
		Shader() : m_handle(0) {};
		Shader(GLuint handle) :
			m_handle(handle) {}
		GLuint handle() const { return m_handle; }

		template<typename UniformType>
		void uniform(const std::string& name, const UniformType& value) const;

		static Shader create(
			const std::string& vertex_shader_src = std::string{},
			const std::string& fragment_shader_src = std::string{});
	private:
		GLuint m_handle;
		template<typename UniformType>
		void uniform(const GLuint uniform_location, const UniformType& value) const;
	};

	template<typename UniformType>
	inline void Shader::uniform(const std::string & name, const UniformType& value) const
	{
		auto location = glGetUniformLocation(m_handle, name.c_str());
		uniform(location, value);
	}
	template<>
	inline void Shader::uniform(const GLuint uniform_location, const glm::mat2& value) const
	{
		glUniformMatrix2fv(uniform_location, 1, GL_FALSE, &value[0][0]);
	}
	template<>
	inline void Shader::uniform(const GLuint uniform_location, const glm::mat4& value) const
	{
		glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &value[0][0]);
	}

	template<>
	inline void Shader::uniform(const GLuint uniform_location, const int& value) const
	{
		glUniform1i(uniform_location, value);
	}
}