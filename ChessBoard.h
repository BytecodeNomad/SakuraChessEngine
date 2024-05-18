#pragma once
#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include "PieceType.h"
#include "Move.h"
#include "GameStatus.h"
class ChessBoard
{
public:
	static std::array<std::array<PieceType, 8>, 8> board;
	static std::vector<Move> move_history;
	static uint64_t move_counter;
	static std::vector<std::array<int,2>> en_passant;
	static int getCurrentPlayer(uint64_t);
	static void parseFEN(std::string);
	static void printBoard();
	static void initialize(std::string);
	static std::vector<std::array<int,2>> leapingPieces(PieceType, std::array<int,2>);
	static bool validateMoves(std::array<int, 2> next_pos);
	static std::string pieces;
	static void makeMove(std::string move_pos);
	static std::vector<std::array<int, 2>> knightMoves(PieceType piece, std::array<int, 2> start_pos);
	static std::vector<std::array<int, 2>> pawnMoves(PieceType piece, std::array<int, 2> start_pos);
	static PieceType promotePawn(PieceType piece, std::array<int, 2> end_pos, char option);
	static std::vector<std::array<int, 2>> kingMoves(PieceType piece, std::array<int, 2> start_pos);
	static std::vector<std::array<int, 2>> slidingAttacks(PieceType piece, std::array<int, 2> start_pos);
	static std::vector<Move> generateMoves();
	static void doMove(std::string move_pos);
	static std::vector<Move> generateLegalMoves();
	static void undoMove();
	static std::array<int, 2> convertPosStringToArray(std::string str_pos);
	static std::array<std::array<int, 2>, 2> convertMoveStringToArray(std::string move_pos);
	static std::string convertPosArraytoMoveString(std::array<int, 2> pos);

	static std::vector<std::array<int, 2>> positionsStartOfGame(std::array<std::array<int, 2>, 2>& start_game_offset,
		std::array<int, 2>& start_pos, int, char);
	static void addToEnPassantList(std::array<int,2>&, std::array<int,2>&, PieceType, char, int);
	static void resolveEnPassantMoves(std::array<int, 2>& start_pos);
}; 

