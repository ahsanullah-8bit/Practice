#include "SceneHandler.h"

#include <iostream>
#include <algorithm>

namespace Tic_Tak_Toe
{
	SceneHandler::SceneHandler() : board(GRID_SIZE, std::vector<char>(GRID_SIZE, ' '))
	{

	}

	void SceneHandler::update(const sf::Vector2i &clicked_cell)
	{
		if (clicked_cell.x < 0 || clicked_cell.y < 0)
			return;

		if (rounds_left && !isAITurn)
		{
			// Check if filled already
			if (board[clicked_cell.y][clicked_cell.x] != ' ')
				return;

			// Action is taken
			board[clicked_cell.y][clicked_cell.x] = 'X';
			isAITurn = true;
			rounds_left--;
		}

		if (rounds_left && isAITurn)
		{
			int x;
			int y;
			do
			{
				x = rand() % GRID_SIZE;
				y = rand() % GRID_SIZE;
			} while (board[y][x] != ' ');

			board[y][x] = 'O';
			isAITurn = false;
			rounds_left--;
		}

		if (winnerChar == 'N')
		{
			if (isTheWinner('X'))
				winnerChar = 'X';
			else if (isTheWinner('O'))
				winnerChar = 'O';
		}

	}

	// Already uses winnerChar, Board and others...
	bool SceneHandler::isTheWinner(char player)
	{
		// Sides
		for (int x = 0; x < GRID_SIZE; x++)
			if (board[0][x] == player && board[1][x] == player && board[2][x] == player ||
				board[x][0] == player && board[x][1] == player && board[x][2] == player)
				return true;
		
		// Diagonal
		if (board[0][0] == player && board[1][1] == player && board[2][2] == player ||
			board[0][2] == player && board[1][1] == player && board[2][0] == player)
			return true;

		return false;
	}

	void SceneHandler::restart()
	{
		isAITurn = false;
		winnerChar = 'N';
		rounds_left = 9;
		
		// Clear the board
		for (auto& vec : board)
			vec.clear();
	}

	// Draws the Board
	void SceneHandler::drawToConsole()
	{
		system("cls");
		for (const auto& vec : board)
		{
			for (const auto& ch : vec)
				std::cout << ch << ' ';

			std::cout << '\n';
		}
	}
}