#pragma once

#include "Application.h"
#include "graphics/Camera.h"
#include "gamelogic/Actor.h"

#include <unordered_map>

namespace dgl 
{

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
	Actor m_cube;

	std::unordered_map<std::string, Shader> shader_lib;
	std::unordered_map<std::string, Texture> texture_lib;

	bool m_close = false;
};

}