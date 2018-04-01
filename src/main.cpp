// main.cpp : Defines the entry point for the application
#include "Game.h"

#include <iostream>

int main(int argc, char** argv)
{
	dgl::WindowSettings window_config;
	dgl::Game game(window_config);
	game.run();
	return 0;
}

