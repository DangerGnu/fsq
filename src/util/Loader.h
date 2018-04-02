#pragma once

// Contains all sorts of loading functions

#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <memory>
#include <tuple>
#include <algorithm>

#include "sdl2/SDL.h"
#include "sdl_image/SDL_image.h"

#include "graphics/GL_Headers.h"
#include "graphics/Geometry.h"

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

	std::vector<std::string> split(const std::string& line, const std::string& delimiter)
	{
		auto tokens = std::vector<std::string>{};
		auto prev = size_t{ 0 };
		auto pos = size_t{ 0 };
		do
		{
			pos = line.find(delimiter, prev);
			if (pos == std::string::npos) 
				pos = line.length();
			auto token = line.substr(prev, pos - prev);
			if (!token.empty()) 
				tokens.emplace_back(token);
			prev = pos + delimiter.length();
		} while (pos < line.length() && prev < line.length());
		return tokens;
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
	// home cooked simple obj loader
	inline Geometry load_object(const std::string& path)
	{
		// open file
		std::ifstream t(path, std::ios::in);
		if (!t.is_open())
		{
			// todo output error
			throw std::invalid_argument("Reading from text file failed. Make sure the file path is correct.");
		}
		std::stringstream buffer;
		buffer << t.rdbuf();
		std::string line;

		// parse contents
		std::vector<glm::vec4> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec4> normals;
		std::vector<std::tuple<GLuint, GLuint, GLuint>> faces;

		while (std::getline(buffer, line))
		{
			if (line.front() == '#') // comment - continue
				continue;
			const auto contents = split(line, " ");
			const auto type = contents.front(); // split returns at least 1 element.
			if (type == "o")
			{
				// ignore - we only load single object objs for now
			}
			else if (type == "v") // thing is a vertex
			{
				vertices.emplace_back(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3]), 1.0f);
			}
			else if (type == "vt") // thing is a uv
			{
				uvs.emplace_back(std::stof(contents[1]), std::stof(contents[2]));
			}
			else if (type == "vn")
			{
				normals.emplace_back(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3]), 0.0f);
			}
			else if (type == "s")
			{
				// ignore - we don't care about seperate surfaces
			}
			else if (type == "f")
			{
				assert(contents.size() == 4);
				// this is complicated ... 
				auto put_tuple = [&faces](const std::string& str) {
					const auto contents = split(str, "/");
					assert(contents.size() == 3); // we only want textured objects with normals. period. Need f 1/2/3 like this
					faces.emplace_back(std::stoi(contents[0]), std::stoi(contents[1]), std::stoi(contents[2]));
				};
				put_tuple(contents[1]);
				put_tuple(contents[2]);
				put_tuple(contents[3]);
			}
		}
		//assert(vertices.size() == normals.size()); // nope - a vertex can have multiple texture coordinates -> duplicate it then.
		// reorder contents to line up data
		// uvs tell the number of vertices!
		auto aligned_vertices = std::vector<glm::vec4>{};
		auto aligned_uvs = std::vector<glm::vec2>{};
		auto aligned_normals = std::vector<glm::vec4>{};

		Geometry geo;
		geo.indices.reserve(faces.size());

		// step1: remove all duplicates from the face buffer.
		// step2: put all vertices according to their indices into the aligned buffers

		auto face_buffer = std::vector<std::tuple<GLuint, GLuint, GLuint>>{};
		// remove duplicates from faces
		for (const auto& face : faces) {
			auto found = std::find_if(begin(face_buffer), end(face_buffer), [&](const auto& el) { 
				GLuint a, b, c, a2, b2, c2;
				std::tie(a, b, c) = face;
				std::tie(a2, b2, c2) = el;
				return a == a2 && b == b2 && c == c2; 
			});
			if (found == end(face_buffer)) {
				face_buffer.emplace_back(face);
			}
		}
		// put all the face vertices into the aligned buffers.
		for (const auto& face : face_buffer)
		{
			GLuint vi, ti, ni;
			std::tie(vi, ti, ni) = face;
			aligned_vertices.emplace_back(vertices[vi-1]);
			aligned_uvs.emplace_back(uvs[ti-1]);
			aligned_normals.emplace_back(normals[ni-1]);
		}
		// how to remap the indices properly?
		for (const auto& face : faces)
		{
			auto found = std::find_if(begin(face_buffer), end(face_buffer), [&](const auto& el) { 
				GLuint a, b, c, a2, b2, c2;
				std::tie(a, b, c) = face;
				std::tie(a2, b2, c2) = el;
				return a == a2 && b == b2 && c == c2;
			});
			//assert(found != end(face_buffer)); // should find face in facebuffer
			auto index = found - begin(face_buffer);
			geo.indices.emplace_back(index);
		}
		for (int i = 0; i < face_buffer.size(); ++i) {
			geo.vertices.emplace_back(dgl::Vertex{ aligned_vertices[i], aligned_normals[i], glm::vec4{ 0.5f, 0.5f, 0.5f, 1.0f }, aligned_uvs[i] });
		}
		return geo;
	}
}