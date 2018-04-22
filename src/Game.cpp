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
	const int grid_size = 30;
	const int grid_spacing = 10;
	m_grid = Actor{ glm::vec3(0.0f), scene_primitives::cube_grid(0.5, grid_size, grid_size, grid_size, grid_spacing, color_shader) };

	// Set the scene camera. 
	m_cam = Camera{ glm::vec3{ 0.0, -10.0, 5.0 } }; // set the camera position

	SDL_WarpMouseInWindow(m_window, m_settings.width / 2, m_settings.height / 2);
	//SDL_ShowCursor(0);
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
			case 27: // esc
			{
				m_key_state.esc = true;
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
			case 27: // esc
			{
				m_key_state.esc = false;
				break;
			}
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			m_mouse_state.x = e.motion.x - m_settings.width / 2;
			m_mouse_state.y = e.motion.y - m_settings.height / 2;
			//SDL_WarpMouseInWindow(m_window, m_settings.width / 2, m_settings.height / 2);
			break;
		}
		default:
			return;
		}
	}
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

// updating the game state. This means moving actors, calculating combat, etc etc
// This is the most interesting function because this is the place were we 
// control what actually happens in our game
// It is also very hard for you to break things here - EXPERIMENT!
void dgl::Game::update()
{
	m_game_time.tick();
	auto elapsed_time = m_game_time.elapsed();
	std::cout << elapsed_time << std::endl;
	const double speed = elapsed_time * 0.004; // speed depends on the time since the last frame
	const double roll_speed = elapsed_time * 2;
	if (m_key_state.a)
	{
		m_cam.m_orientation = m_cam.m_orientation * glm::rotate<float>(glm::quat{ 1,0,0,0 }, -roll_speed, glm::vec3{ 0,1,0 });
	}
	if(m_key_state.d)
	{
		m_cam.m_orientation = m_cam.m_orientation * glm::rotate<float>(glm::quat{ 1,0,0,0 }, roll_speed, glm::vec3{ 0,1,0 });
	}
	if (m_key_state.w)
	{
		m_player.pos += glm::vec3(m_player.orientation * glm::vec4{ 0.0, 0.2, 0.0, 0.0 });
	}
	if (m_key_state.s)
	{
		m_player.pos += glm::vec3(m_player.orientation * glm::vec4{ 0.0, -0.2, 0.0, 0.0 });
	}
	if (m_key_state.z)
	{
		m_player.pos = glm::vec3{ 0.0,0.0,0.0 };
	}
	if (m_key_state.esc)
	{
		m_close = true;
	}

	auto speed_x = -(double)m_mouse_state.x;
	auto speed_y = (double)m_mouse_state.y;

	auto x_rot = glm::rotate<float>(glm::quat(1,0,0,0), -speed_y * 0.0001, glm::vec3(1, 0, 0));
	auto z_rot = glm::rotate<float>(glm::quat(1, 0, 0, 0), speed_x * 0.0001, glm::vec3(0, 0, 1));
	auto y_strafe_rot = glm::rotate<float>(glm::quat(1, 0, 0, 0), -speed_x * 0.001, glm::vec3(0, 1, 0));

	m_cam.m_orientation =  m_cam.m_orientation * z_rot * x_rot;
	m_player.orientation = m_cam.m_orientation * y_strafe_rot;
	auto cam_p = m_cam.m_orientation * glm::vec4(-speed_x * 0.01, -8, 4 - speed_y * 0.01, 1);
	//m_cam.m_orientation = m_player.orientation;
	m_cam.m_position = m_player.pos +  glm::vec3(cam_p.x, cam_p.y, cam_p.z);
	m_cam.m_target = m_player.pos + glm::vec3(m_player.orientation * glm::vec4(0,20,0,1));
	
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
	draw(m_grid);
}
