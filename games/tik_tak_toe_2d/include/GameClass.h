// Handles the main loop
#pragma once
#include "SceneHandler.h"
#include "Renderer.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace tictaktoe
{

class GameClass {
	friend Renderer;
public:
	GameClass();
	void play();

private:
	SceneHandler scene;
	Renderer renderer;

	sf::RenderWindow window;
};

}