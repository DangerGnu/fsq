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
	const std::string resource_path = "..\\..\\..\\resources\\";
	const std::string shaders_path = "..\\..\\..\\shader\\";
};

dgl::Game::Game(const WindowSettings & settings) : Application(settings){}

void dgl::Game::setup()
{
	// just some constant strings to find the resources
	const ResourceConfig cfg;
	// build a shader - this is a program that runs on the gpu and actually renders stuff
	auto color_shader = Shader::create(
		dgl::read_text(cfg.shaders_path + "simple_in_out.vert"),
		dgl::read_text(cfg.shaders_path + "simple_in_out.frag"));

	auto texture_shader = Shader::create(
		dgl::read_text(cfg.shaders_path + "simple_texture.vert"),
		dgl::read_text(cfg.shaders_path + "simple_texture.frag"));

	// load the background image into memory
	//auto level_image = dgl::load_texture(cfg.resource_path + "landscape.png");
	// ... and create a texture from it (the image is in video-ram now)
	//auto level_texture = Texture(*level_image);
	// Todo throw this into a yaml file that describes the resources of a game object
	auto player_geo = load_object(cfg.resource_path + "spaceship.obj");
	auto player_batch = dgl::Batch{ texture_shader, player_geo };
	auto player_tex = dgl::Texture{ *load_texture(cfg.resource_path + "spaceship_tex.png") };
	auto player_scene_obj = SceneObj{ player_tex, player_batch };
	m_player = Actor{ glm::vec3(0.0f), std::move(player_scene_obj) };
	//m_player.color = glm::vec3{ 0,1.0,0 };

	Actor obstacle = Actor{ glm::vec3(0.0f), scene_primitives::colored_cube(1.0f,{ 0.0f,0.0f,1.0f,1.0f },{}, color_shader) };
	const int grid_size = 10;
	const int grid_spacing = 10;
	for (int i = 0; i < grid_size; ++i)
	{
		for (int j = 0; j < grid_size; ++j)
		{
			m_obstacles.emplace_back(glm::vec3(grid_spacing * i - grid_size*grid_spacing*0.5, grid_spacing * j - grid_size*grid_spacing*0.5, 0), scene_primitives::colored_cube(1.0f, { 1,1,1,1 }, {}, color_shader));
		}
	}

	// Set the scene camera. 
	m_cam = Camera{ glm::vec3{ 0.0, -10.0, 5.0 } }; // set the camera position
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
		case SDL_KEYDOWN: // key pressed
		{
			// print the key-code and the position of the mouse when the key was pressed
			int key = (int)e.key.keysym.sym;
			switch (key)
			{
			case 97: // a
			{
				m_key_state.a = true;
				// bewege würfel nach links
				break;
			}
			case 119: // w
			{
				m_key_state.w = true;
				break;
			}
			case 115: // s
			{
				m_key_state.s = true;
				break;
			}
			case 100: // d
			{
				m_key_state.d = true;
				break;
			}
			case 122: // z 
			{
				m_key_state.z = true;
				break;
			}
			}
			break;
		}
		case SDL_KEYUP:
		{
			int key = (int)e.key.keysym.sym;
			switch (key)
			{
			case 97: // a
			{
				m_key_state.a = false;
				// bewege würfel nach links
				break;
			}
			case 119: // w
			{
				m_key_state.w = false;
				break;
			}
			case 115: // s
			{
				m_key_state.s = false;
				break;
			}
			case 100: // d
			{
				m_key_state.d = false;
				break;
			}
			case 122: // z 
			{
				m_key_state.z = false;
				break;
			}
			}
			break;
		}
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
	static float time = 0;
	int x = 1;
	
	if (m_key_state.a)
	{
		m_player.pos += glm::vec3{ -0.2, 0.0, 0.0 };
	}
	if(m_key_state.d)
	{
		m_player.pos += glm::vec3{ 0.2, 0.0, 0.0 };
	}
	if (m_key_state.w)
	{
		m_player.pos += glm::vec3{ 0.0, 0.2, 0.0 };
	}
	if (m_key_state.s)
	{
		m_player.pos += glm::vec3{ 0.0, -0.2, 0.0 };
	}
	if (m_key_state.z)
	{
		m_player.pos = glm::vec3{ 0.0,0.0,0.0 };
	}
	m_cam.m_position += glm::vec3{ 0.0, 0.0, sin(time) };
	time += 0.01;
}

// the rendering function that takes care of making everything appear on screen
// not much to do here. Just call the draw function on each object in the scene
// remark: coordinate system origin is in the center of the screen!
void dgl::Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// tell openGL to clear the previous screen
	auto draw = [this](const auto& actor) {
		dgl::draw(actor, m_cam.view(), m_cam.projection());
	};
	draw(m_player);
	
	for (const auto& obstacle : m_obstacles)
	{
		dgl::draw(obstacle, m_cam.view(), m_cam.projection());
	}
}
