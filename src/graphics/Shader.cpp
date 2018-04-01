#include "Shader.h"

// for debug
std::string shader_name(const GLuint shader_type)
{
	switch (shader_type)
	{
		case GL_VERTEX_SHADER: return "Vertex Shader";
		case GL_FRAGMENT_SHADER: return "Fragment Shader";
		case GL_GEOMETRY_SHADER: return "Geometry Shader";
		// @todo: others
		default: return "";
	}

}
// for debug
void check_compilation(const GLuint shader_handle, const GLuint shader_type = 0)
{
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader_handle, 512, NULL, infoLog);
		std::cout << "ERROR :: " << shader_name(shader_type) <<  " :: COMPILATION FAILED\n" << infoLog << std::endl;
	}
}

GLuint compile_shader(const GLuint shader_type, const char* shader_src)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader);
#ifdef _DEBUG
	check_compilation(shader, shader_type);
#endif
	return shader;
}

GLuint link_program(const GLuint vertex_shader, const GLuint fragment_shader)
{
	auto shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);
#ifdef _DEBUG
	check_compilation(shader_program);
#endif
	// @todo: we may want to reuse this ... maybe later but not now.
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return shader_program;
}

dgl::Shader dgl::Shader::create(const std::string & vertex_shader_src, const std::string & fragment_shader_src)
{
	// compile the sources
	auto vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src.c_str());
	auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src.c_str());
	// link them into a program
	auto shader_program = link_program(vertex_shader, fragment_shader);
	// return handle as shader
	return Shader{ shader_program };
}
