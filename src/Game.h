#pragma once

#include "Application.h"
#include "graphics/Camera.h"
#include "gamelogic/Actor.h"
#include "gamelogic/GameTime.h"

#include <unordered_map>
#include <vector>

namespace dgl 
{

struct KeyState {
	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;
	bool z = false;
	bool shift = false;
	bool esc = false;
};

struct MouseState {
	int x = 0;
	int y = 0;
	bool left_click = false;
	bool right_click = false;
};

class Game : public Application
{
public:
	Game(const WindowSettings& settings);
	void run();
private:
	void setup();
	void handle_input();
	void update();
	void draw();

	Camera m_cam;
	Actor m_player;
	Actor m_grid;
	KeyState m_key_state;
	MouseState m_mouse_state;

	
	std::unordered_map<std::string, Shader> shader_lib;
	std::unordered_map<std::string, Texture> texture_lib;

	Time m_game_time;

	bool m_close = false;
};

}