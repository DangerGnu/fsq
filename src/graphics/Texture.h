#pragma once

#include <memory>

#include "sdl2/SDL.h"
#include "GL_Headers.h"

namespace dgl
{
	class Texture
	{
	public:
		Texture() : m_tex_handle(0) {}
		Texture(const SDL_Surface& image);
		void bind(GLuint tex_unit) const;
	private:
		GLuint m_tex_handle = 0;
	};
}
