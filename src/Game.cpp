#include "Game.h"
#include "util/Loader.h"

#include "graphics/Geometry.h"
#include "graphics/Primitives.h"
#include "graphics/DrawFunctions.h"
#include "graphics/ScenePrimitives.h"

#include "glm/gtc/constants.hpp"

// @flo start developing here :)

struct ResourceConfig
{
	const std::string resource_path = "..\\..\\resources\\";
	const std::string shaders_path = "..\\..\\shader\\";
};

dgl::Game::Game(const WindowSettings & settings) : Application(settings){}

void dgl::Game::setup()
{
	// just some constant strings to find the resources
	const ResourceConfig cfg;
	// build a shader - this is a program that runs on the gpu and actually renders stuff
	auto simple_shader = Shader::create(
		dgl::read_text(cfg.shaders_path + "simple_in_out.vert"),
		dgl::read_text(cfg.shaders_path + "simple_in_out.frag"));

	// load the background image into memory
	//auto level_image = dgl::load_texture(cfg.resource_path + "landscape.png");
	// ... and create a texture from it (the image is in video-ram now)
	//auto level_texture = Texture(*level_image);

	m_cube = Actor{ glm::vec3(0.0f), scene_primitives::colored_cube(1.0f, {0.0f,0.0f,1.0f,1.0f}, {}, simple_shader) };
	// Set the scene camera. 
	m_cam = Camera{ glm::vec3{ 2.0f, 2.0f, 2.0f } }; // set the camera position
}

// do all the stuff we need to do only once on startup + the game loop
void dgl::Game::run()
{
	setup();
	// start game loop and keep it running until the program is shut down.
	while (!m_close)
	{
		handle_input();
		update();
		draw();
		SDL_GL_SwapWindow(m_window);
	}
}

// input can be keyboard input, mouse input, or window events
/* 
	SDL uses something called an event queue. Everytime the user performs an action
	(moving mouse, clicking, pressing buttons) an event holding information about that action
	is pushed into an event queue. In our game loop we poll the events from that queue every frame. 
*/
void dgl::Game::handle_input()
{
	SDL_Event e;
	// polling from the event queue - e is passed in as a reference and filled with information
	while (SDL_PollEvent(&e))	
	{
		// switch(_){case _:} is a better way of writing
		// if(e.type == SDL_QUIT){ ... }else if(e.type == SDL_MOUSEBUTTONDOWN){ ... }else if
		// we're switching over the type of the event
		switch (e.type)	
		{
		case SDL_QUIT:	// if the event has the value SDL_QUIT ...
			m_close = true;	// set the close variable to true and the app shuts down
			break;	// and break out of the switch-statement (NEVER forget this!)
		case SDL_MOUSEBUTTONDOWN:	
			// print the screen coordinates
			std::cout << (int)e.button.button << std::endl;
			std::cout << e.button.x << " " << e.button.y << std::endl;
			break;
		case SDL_MOUSEBUTTONUP:	// do nothing but you'll need this soon enough ;) 
			break;
		case SDL_KEYDOWN:
			// print the key-code and the position of the mouse when the key was pressed
			std::cout << (int)e.key.keysym.sym << std::endl;
			int x, y;
			SDL_GetMouseState(&x, &y);
			std::cout << x << " " << y << std::endl;
		case SDL_KEYUP:
			break;
		default:
			return;
		}
	}
}

// updating the game state. This means moving actors, calculating combat, etc etc
// This is the most interesting function because this is the place were we 
// control what actually happens in our game
// It is also very hard for you to break things here - EXPERIMENT!
void dgl::Game::update()
{
}

// the rendering function that takes care of making everything appear on screen
// not much to do here. Just call the draw function on each object in the scene
// remark: coordinate system origin is in the center of the screen!
void dgl::Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);	// tell openGL to clear the previous screen
	dgl::draw(m_cube, m_cam.view_projection());
}
