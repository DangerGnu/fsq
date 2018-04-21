#include "Application.h"

#include <iostream>
// SDL
#include "graphics/CleanUpSDL.h"
#include "sdl2/SDL_keyboard.h"
// Scene
#include "graphics/Camera.h"
#include "graphics/Geometry.h"
#include "graphics/Primitives.h"
#include "graphics/Shader.h"
#include "graphics/Batch.h"
// Asset Loading
#include "util/Loader.h"
// Rendering
#include "graphics/DrawFunctions.h"
// Game
#include "gamelogic/MovementFunctions.h"

// fugly init code
dgl::Application::Application(const WindowSettings & settings) : m_settings(settings)
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return;
	}
	// Create Window
	m_window = SDL_CreateWindow(
		m_settings.title.c_str(), m_settings.posX, m_settings.posY,
		m_settings.width, m_settings.height, m_settings.flags);
	if (m_window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return;
	}
	// Set OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Create OpenGL context
	m_gl_context = SDL_GL_CreateContext(m_window);
	if (m_gl_context == nullptr)
	{
		std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
	}
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		std::cout << "GLEW could not be initialized " << glewGetErrorString(glewError) << std::endl;
		return;
	}
	// Use Vsync
	if (m_settings.vsync && SDL_GL_SetSwapInterval(1) != 0)
	{
		std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
	}
	std::cout << "Enabling Depth Test" << std::endl;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

dgl::Application::~Application()
{
	// clean all the stuff
	cleanup_sdl(m_window);
	SDL_GL_DeleteContext(m_gl_context);
	SDL_QUIT;
}
