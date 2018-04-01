#pragma once

// Contains all sorts of loading functions

#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <memory>

#include "sdl2/SDL.h"
#include "sdl_image/SDL_image.h"

#include "graphics/GL_Headers.h"

namespace
{
	// flip SDL_Surface in Y direction
	void flipY(SDL_Surface& surf)
	{
		const int width = surf.w;
		const int height = surf.h;
		const int bytes_per_pixel = surf.format->BytesPerPixel;
		const int stride = bytes_per_pixel * width;

		char* origin = static_cast<char*>(surf.pixels); // interpret void* as pointer of bytes
		std::vector<char> buffer(stride);
		// iterate through rows from top to mid and swap each row with the bottom half
		for (int i = 0; i < height / 2; ++i)
		{
			// offset in linear pixel buffer
			const int offset_top = stride * i;
			const int offset_bottom = stride * (height - (i + 1));
			// positions within top and bottom rows
			const auto top_left = origin + offset_top;
			const auto top_right = origin + offset_top + stride;
			const auto bottom_left = origin + offset_bottom;
			const auto bottom_right = origin + offset_bottom + stride;
			// just make sure all is correct here. I don't trust myself.
			assert((bottom_left - origin) < (bottom_right - origin));
			assert((top_left - origin) < (top_right - origin));
			assert((top_right - origin) <= (bottom_left - origin));
			assert((bottom_right - origin) <= (width * height * bytes_per_pixel));
			// actual swap
			// fugly pointer arithmetic ... no other chance though since Pixels are void*
			std::copy(top_left, top_right, std::begin(buffer));								
			std::copy(bottom_left, bottom_right, top_left);
			std::copy(std::begin(buffer), std::end(buffer), bottom_left);
		}
	}
}

namespace dgl 
{
	inline std::string read_text(const std::string& file_path)
	{
		std::ifstream t(file_path, std::ios::in);
		if (!t.is_open())
		{
			// todo output error
			throw std::invalid_argument("Reading from text file failed. Make sure the file path is correct.");
		}
		std::stringstream buffer;
		buffer << t.rdbuf();
		return buffer.str();
	}
	
	inline std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> load_texture(const std::string& path)
	{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			std::cout << "Something wrong with IMG_INIT" << IMG_GetError() << std::endl;
		}
		auto surf = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(IMG_Load(path.c_str()), &SDL_FreeSurface);
		if (surf == nullptr)
		{
			std::cout << "loading image failed: " << IMG_GetError() << std::endl;
		}
		std::cout << "pixel format: " << SDL_GetPixelFormatName(surf->format->format);
		flipY(*surf);

		return surf;
	}
}