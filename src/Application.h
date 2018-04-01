#pragma once

#include "sdl2/SDL.h"
#include <string>


namespace dgl
{
	struct WindowSettings
	{
		std::string title = "fsq";
		int width = 800;
		int height = 600;
		int posX = 100;
		int posY = 100;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
		bool vsync = true;
	};

	class Application
	{
	public:
		Application(const WindowSettings& settings);
		~Application();
		SDL_Window* window_handle() { return m_window; }
		SDL_GLContext gl_context() { return m_gl_context; }
	protected:
		WindowSettings m_settings;
		SDL_Window* m_window = nullptr;
		SDL_GLContext m_gl_context = nullptr;
	};
}