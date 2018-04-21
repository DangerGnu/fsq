#pragma once

#include "Application.h"
#include "graphics/Camera.h"
#include "gamelogic/Actor.h"

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
	KeyState m_key_state;
	std::vector<Actor> m_obstacles;
	
	std::unordered_map<std::string, Shader> shader_lib;
	std::unordered_map<std::string, Texture> texture_lib;

	bool m_close = false;
};

}