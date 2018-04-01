#include "Game.h"
#include "util/Loader.h"

#include "graphics/Geometry.h"
#include "graphics/Primitives.h"
#include "graphics/DrawFunctions.h"
#include "graphics/ScenePrimitives.h"

#include "glm/gtc/constants.hpp"

struct ResourceConfig
{
	const std::string resource_path = "..\\..\\resources\\";
	const std::string shaders_path = "..\\..\\shader\\";
};

dgl::Game::Game(const WindowSettings & settings) : Application(settings){}

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

void dgl::Game::setup()
{
	// just some constant strings to find the resources
	const ResourceConfig cfg;
	// build a shader - this is a program that runs on the gpu and actually renders stuff
	auto simple_shader = Shader::create(
		dgl::read_text(cfg.shaders_path + "simple_in_out.vert"),
		dgl::read_text(cfg.shaders_path + "simple_in_out.frag"));

	// load the background image into memory
	auto level_image = dgl::load_texture(cfg.resource_path + "landscape.png");
	// ... and create a texture from it (the image is in video-ram now)
	auto level_texture = Texture(*level_image);
	// do the same for the pelican
	auto pelican_image = dgl::load_texture(cfg.resource_path + "test_alpha.png");
	auto pelican_texture = Texture(*pelican_image);

	// Create the "Map"
	// Look at the definition of Map (klick on Map and press F12) to see what the parameters mean
	m_level = Map{ 2.0f , 2.0f, level_texture , simple_shader };
	// Create the pelican 
	// Look at the definition of Actor!
	m_pelican = Actor{ glm::vec3(0.0f), scene_primitives::textured_quad(1.0f,0.5f, pelican_texture, simple_shader) };
	m_cube = Actor{ glm::vec3(0.0f), scene_primitives::colored_cube(1.0f, {0.0f,0.0f,1.0f,1.0f}, {}, simple_shader) };
	// Set the scene camera. 
	m_cam = Camera{ glm::vec3{ 2.0f, 2.0f, 2.0f } }; // set the camera position

}

// input can be keyboard input, mouse input, or window events
/* 
	SDL uses something called an event queue. Everytime the user performs an action
	(moving mouse, clicking, pressing buttons) an event holding information about that action
	is pushed into an event queue. In our game loop we poll the events from that queue every frame
	and to them. 
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

/* ============================================================================
	Challenge 1: Moving Pelicans

	When you run the game now - the pelican is majestically moving around in a
	circle (if it isn't call me an idiot and urge me to fix bugs!). The goal of
	this challenge is to make the pelican move to the location of the cursor on
	mouse-click. We'll get there in small steps:

	1.1 Experiment
	Fool around with the values of the animation parameters in the Game::update() function
	to get a sense of how they work. Most importantly: understand what value the target vector has
	and how it changes over time. Take note that the coordinate system has its origin at the
	center of the screen.

	1.2 Change the movement pattern
	Circles are dull. Change the movement pattern! (commit your changes for each pattern separately to git)
		1.2.1 make him move up and down only
		1.2.2 make him move in a sine wave from position (2.0f, 0.0f, 0.0f) to (-2.0f, 0.0f, 0.0f)
		  restart the movement when he has reached his destination.
		  Increase the frequency of the sine wave by multiplying the argument to the sine wave function
		  by a constant factor.
		1.2.3 make him move in a square
		1.2.4 make him move from his initial position to position (1.0f, 0.5f, 0.0f) and then stop

		hints:
		1.2.1	x value of target-vector must not change
		1.2.2	anim_time is your x value. set the correct start value for anim time
				and reset anim_time once it's larger than your target-value
		1.2.3	there's no math function for that. You can use if-statements over anim_time
				to figure out the appropriate target vector based on anim time.
		1.2.4	save the initial position of the pelican (m_pelican.pos) in a static variable.
				To get movement based on time you need an interpolation function.
				Linear interpolation looks like this:
				f(t) = initial_position * (1.0f-t) + goal_position * t
				where t is between 0 and 1
	... tbc

   ==========================================================================*/

// updating the game state. This means moving actors, calculating combat, etc etc
// This is the most interesting function because this is the place were we 
// control what actually happens in our game
// It is also very hard for you to break things here - EXPERIMENT!
void dgl::Game::update()
{
	// static means the value will be initialized only once. 
	// This means it will retain its value even when the function is called again
	const float amplitude = 0.5f;
	const float speed = 3.0f;
	static float anim_time = 0.0f;
 
	m_pelican.pos = glm::vec3{ -amplitude * sin(anim_time), -amplitude * cos(anim_time), 0.0f };
	auto view_target = glm::vec3{ -glm::sign(cos(anim_time)), 0.0f, 0.0f };
	m_pelican.view = view_target;
	anim_time += 0.016f * speed;	// advance time
}

// the rendering function that takes care of making everything appear on screen
// not much to do here. Just call the draw function on each object in the scene
// remark: coordinate system origin is in the center of the screen!
void dgl::Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);	// tell openGL to clear the previous screen
	//dgl::draw(m_level, m_cam.view_projection());	// draw the map
	//dgl::draw(m_pelican, m_cam.view_projection());	// draw the pelican on top of that
	dgl::draw(m_cube, m_cam.view_projection());
}
