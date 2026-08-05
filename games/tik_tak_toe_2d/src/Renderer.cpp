#include "Renderer.h"

#include <filesystem>


namespace Tic_Tak_Toe
{
	Renderer::Renderer() : texture()
	{	
		// Loading textures
        std::filesystem::path sprite_sheet_path = std::filesystem::current_path() / "resources/tic-tac-toe_assets.png";
		texture.loadFromFile(sprite_sheet_path.string());

		background = sf::Sprite(texture, sf::IntRect(125, 3, 63, 112));
		background.setScale(SCALE_FACTORx, SCALE_FACTORy);

		po_sprite = sf::Sprite(texture, sf::IntRect(210, 96, 17, 17));
		po_sprite.setScale(SCALE_FACTORx, SCALE_FACTORy);

		px_sprite = sf::Sprite(texture, sf::IntRect(191, 96, 17, 17));
		px_sprite.setScale(SCALE_FACTORx, SCALE_FACTORy);
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

				if (board[y][x] == 'X')
				{
					px_sprite.setPosition(x_pos, y_pos);
					window.draw(px_sprite);
				}
				else if (board[y][x] == 'O')
				{
					po_sprite.setPosition(x_pos, y_pos);
					window.draw(po_sprite);
				}
			}

		window.display();
	}
}
