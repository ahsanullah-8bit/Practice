#pragma once
#include "SceneHandler.h"
#include <SFML/Graphics.hpp>
#include <vector>


namespace Tic_Tak_Toe
{
	class Renderer
	{
	public:
		Renderer();
		void render(const SceneHandler& scene, sf::RenderWindow& window);

	//private:

		sf::Texture texture;
		sf::Sprite background;
		sf::Sprite px_sprite;	
		sf::Sprite po_sprite;
	};
}