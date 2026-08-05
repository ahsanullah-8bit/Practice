#pragma once
#include "Constants.h"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

namespace Tic_Tak_Toe
{
	class KeyboardInput
	{
	public:
		// Also closes the window if triggered event.close
		static sf::Vector2i getClickedCell(sf::RenderWindow& window)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

					int x = static_cast<int>((mouse_pos.x / SCALE_FACTORx - PADDING_BOARD_LEFT) / CELL_SIZE);
					int y = static_cast<int>((mouse_pos.y / SCALE_FACTORy - PADDING_BOARD_TOP) / CELL_SIZE);

					if (x >= 0 && y >= 0 && x < GRID_SIZE && y < GRID_SIZE)
						return sf::Vector2i(x, y);
				}
			}

			return {-1, -1};
		}
	};
} 