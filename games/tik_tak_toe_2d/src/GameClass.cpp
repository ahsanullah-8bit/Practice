#include "GameClass.h"
#include "KeyboardInput.h"
#include "Constants.h"

#include <thread>

#include <SFML/Window/VideoMode.hpp>

namespace Tic_Tak_Toe
{
	GameClass::GameClass() : scene(), window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tic-Tac-Toe"), renderer() {}

	void GameClass::play()
	{
		while (window.isOpen())
		{
			scene.update(KeyboardInput::getClickedCell(window));
			renderer.render(scene, window);
			//scene.drawToConsole();

			if (scene.winnerChar != 'N')
				window.close();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}