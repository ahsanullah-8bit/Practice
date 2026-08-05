// Handles the main loop
#pragma once
#include "SceneHandler.h"
#include "Renderer.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Tic_Tak_Toe
{
	class GameClass
	{
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