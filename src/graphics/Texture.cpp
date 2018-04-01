#include "Texture.h"

#include "sdl2/SDL.h"
#include "GL_Headers.h"

using namespace dgl;

Texture::Texture(const SDL_Surface& image)
{
	glGenTextures(1, &m_tex_handle);
	glBindTexture(GL_TEXTURE_2D, m_tex_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.w, image.h, 0, GL_ABGR_EXT, GL_UNSIGNED_BYTE, image.pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// @todo: mip mapping and stuff.
}

void Texture::bind(GLuint tex_unit) const
{
	glActiveTexture(tex_unit);
	glBindTexture(GL_TEXTURE_2D, m_tex_handle);
}
