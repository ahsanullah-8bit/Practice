#pragma once
#include <iostream>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>

#include "Constants.h"

namespace tictaktoe
{

class KeyboardInput {
public:
	// Also closes the window if triggered event.close
	static sf::Vector2i getClickedCell(sf::RenderWindow& window)
	{
		// sf::Event event;
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
			else if (const auto *mouse_click = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouse_click->button == sf::Mouse::Button::Left) {

				}
				sf::Vector2i mouse_pos = mouse_click->position;

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