#include "Renderer.h"

#include <filesystem>
#include <iostream>

namespace tictaktoe
{

Renderer::Renderer()
	: texture()
	, background(texture, sf::IntRect({125, 3}, {63, 112}))
	, po_sprite(texture, sf::IntRect({210, 96}, {17, 17}))
	, px_sprite(texture, sf::IntRect({191, 96}, {17, 17}))
{	
	// Loading textures
	std::filesystem::path sprite_sheet_path = std::filesystem::current_path() / "assets/tic-tac-toe_assets.png";
	if (!texture.loadFromFile(sprite_sheet_path.string()))
		std::cerr << "Error: Failed to load texture file at " << sprite_sheet_path << std::endl;

	background.setScale(sf::Vector2f{SCALE_FACTORx, SCALE_FACTORy});
	po_sprite.setScale(sf::Vector2f{SCALE_FACTORx, SCALE_FACTORy});
	px_sprite.setScale(sf::Vector2f{SCALE_FACTORx, SCALE_FACTORy});
}

void Renderer::render(const SceneHandler& scene, sf::RenderWindow& window)
{
	window.clear(sf::Color::White);
	window.draw(background);

	auto& board = scene.board;
	for (size_t y = 0; y < board.size(); ++y)
		for (size_t x = 0; x < board[y].size(); ++x)
		{
			float x_pos = (PADDING_BOARD_LEFT + x * CELL_SIZE) * SCALE_FACTORx + (PADDING_CELL * x);
			float y_pos = (PADDING_BOARD_TOP  + y * CELL_SIZE) * SCALE_FACTORy + (PADDING_CELL * y);

			if (board[y][x] == 'X') {
				px_sprite.setPosition(sf::Vector2f{x_pos, y_pos});
				window.draw(px_sprite);
			}
			else if (board[y][x] == 'O') {
				po_sprite.setPosition(sf::Vector2f{x_pos, y_pos});
				window.draw(po_sprite);
			}
		}

	window.display();
}

}
