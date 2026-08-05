#pragma once

namespace
{
	const float SCALE_FACTORx = 5.0f;
	const float SCALE_FACTORy = 5.0f;

	const int WINDOW_HEIGHT = 113 * (int)SCALE_FACTORx;
	const int WINDOW_WIDTH = 64 * (int)SCALE_FACTORy;

	const int GRID_SIZE = 3;	// 3x3
	const float CELL_SIZE = 17.0f;	// 17x17

	// Padding of the Board
	const int PADDING_BOARD_LEFT = 4;	// px
	const int PADDING_BOARD_TOP = 35;

	const float PADDING_CELL = 2.0f * SCALE_FACTORx;

	int ROUNDS_LEFT = 9;
}