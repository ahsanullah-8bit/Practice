#pragma once
#include "Constants.h"

#include <SFML/Graphics/Vertex.hpp>
#include <vector>

namespace Tic_Tak_Toe
{
	class SceneHandler
	{
		friend class Renderer;
	public:
		SceneHandler();
		void update(const sf::Vector2i &clicked_cell);
		void restart();

		void drawToConsole();

	private:
		bool isTheWinner(char);


	private:
		std::vector<std::vector<char>> board;

		bool isAITurn = false;
		int rounds_left = 9;

	public:
		char winnerChar = 'N';	// N(Non), P(Player), A(AI)
	};
}