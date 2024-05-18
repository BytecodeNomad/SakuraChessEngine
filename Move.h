#pragma once
#include "PieceType.h"
#include <array>
#include <vector>
class Move
{
public:
	Move(std::array<int, 2> start_pos, std::array<int, 2> target_pos, PieceType moved, PieceType captured) :
	start_square{start_pos}, target_square{target_pos}, moved_piece{moved}, captured_piece{captured} {}
	std::array<int, 2> start_square;
	std::array<int, 2> target_square;
	PieceType captured_piece;
	PieceType moved_piece;
};



