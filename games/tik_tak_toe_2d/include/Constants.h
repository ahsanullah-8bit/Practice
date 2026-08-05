#pragma once

namespace tictaktoe
{
	constexpr float SCALE_FACTORx = 5.0f;
	constexpr float SCALE_FACTORy = 5.0f;

	constexpr int WINDOW_HEIGHT = 113 * (int)SCALE_FACTORx;
	constexpr int WINDOW_WIDTH = 64 * (int)SCALE_FACTORy;

	constexpr int GRID_SIZE = 3;	// 3x3
	constexpr float CELL_SIZE = 17.0f;	// 17x17

	// Padding of the Board
	constexpr int PADDING_BOARD_LEFT = 4;	// px
	constexpr int PADDING_BOARD_TOP = 35;

	constexpr float PADDING_CELL = 2.0f * SCALE_FACTORx;

	constexpr int ROUNDS_LEFT = 9;
}