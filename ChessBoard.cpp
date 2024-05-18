#include "ChessBoard.h"

std::array<std::array<PieceType, 8>, 8> ChessBoard::board;
std::vector<Move> ChessBoard::move_history;
uint64_t counter{ 0 };
std::string ChessBoard::pieces = ".PRNBQKprnbqk";
std::vector<std::array<int, 2>> ChessBoard::en_passant;

void ChessBoard::parseFEN(std::string fen_input) {
	std::istringstream fen_string = std::istringstream(fen_input);
	std::string board_info;
	fen_string >> board_info;

	for (auto& row : board) {
		for (auto& col : row) {
			col = PieceType::EMPTY;
		}
	}
	//"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
	int row = 0;
	int col = 0;
	for (auto& info : board_info) {
		if (info == '/') {
			col = 0;
			row++;
		}
		else if (isdigit(info)) {
			col += info - '0';
		}
		else {
			PieceType p{};
			if (islower(info)) {
				switch (info) {
					case 'r':
						p = PieceType::BLACK_ROOK;
						break;
					case 'n':
						p = PieceType::BLACK_KNIGHT;
						break;
					case 'b':
						p = PieceType::BLACK_BISHOP;
						break;
					case 'q':
						p = PieceType::BLACK_QUEEN;
						break;
					case 'k':
						p = PieceType::BLACK_KING;
						break;
					case 'p':
						p = PieceType::BLACK_PAWN;
						break;
				}
			}
			if (isupper(info)) {
				switch (info) {
					case 'R':
						p = PieceType::WHITE_ROOK;
						break;
					case 'N':
						p = PieceType::WHITE_KNIGHT;
						break;		   
					case 'B':		   
						p = PieceType::WHITE_BISHOP;
						break;		   
					case 'Q':		   
						p = PieceType::WHITE_QUEEN;
						break;		   
					case 'K':		   
						p = PieceType::WHITE_KING;
						break;		   
					case 'P':		   
						p = PieceType::WHITE_PAWN;
						break;
					}
			}

			board[row][col++] = p;
		}

	}
}

void ChessBoard::printBoard() {
	
	for (auto& row : board) {
		for (auto& col : row) {
			std::cout << std::setw(2) << pieces.at(static_cast<size_t>(col));
		}
		std::cout << std::endl;
	}
}


void ChessBoard::initialize(std::string fen_string) {
	counter = 0;
	parseFEN(fen_string);
	
//	printBoard();
}
int ChessBoard::getCurrentPlayer(uint64_t counter) {
	return counter % 2 == 0 ? 1 : 2;
}
std::array<int,2> ChessBoard::convertPosStringToArray(std::string str_pos) {
	std::array<int, 2> board_pos{};
	board_pos[0] = '8' - str_pos[1];
	board_pos[1] = str_pos[0] - 'a';

	return board_pos;

}

std::array<std::array<int, 2>, 2> ChessBoard::convertMoveStringToArray(std::string move_pos) {
	std::string start_str = move_pos.substr(0, 2);
	std::array<int, 2> start_pos = convertPosStringToArray(start_str);

	std::string end_str = move_pos.substr(2, 2);
	std::array<int, 2> end_pos = convertPosStringToArray(end_str);

	return { start_pos, end_pos };
}

std::string ChessBoard::convertPosArraytoMoveString(std::array<int, 2> pos) {
	std::string move_str;
	move_str = 'a' + pos[1];
	move_str += '1' +  7 - pos[0];

	return move_str;
}

void ChessBoard::makeMove(std::string move_pos) {
	
	std::array < std::array<int, 2>, 2> moveArray = convertMoveStringToArray(move_pos);
	std::array<int, 2> start_pos = moveArray[0];
	std::array<int, 2> end_pos = moveArray[1];
	char option = move_pos[4];
	PieceType piece = board[start_pos[0]][start_pos[1]];

	std::cout << "Current move: " << move_pos << ". Piece Type: " << pieces[(int)piece] << std::endl;
	std::cout << "The current turn: " << getCurrentPlayer(counter) << std::endl;
	int int_piece_rep = static_cast<int>(piece);

	char piece_rep = pieces[static_cast<size_t>(piece)];
	char lower_piece_rep = tolower(piece_rep);

	if (getCurrentPlayer(counter) != 1 && isupper(piece_rep)) {
		std::cout << "Wrong Piece Moved! White piece is being moved" << std::endl;

		return;
	}
	if (getCurrentPlayer(counter) != 2 && islower(piece_rep)) {
		std::cout << "Wrong Piece Moved! Black piece is being moved" << std::endl;
		return;
	}

	std::vector<Move> all_possible_moves = generateLegalMoves();
	if (all_possible_moves.empty()) {
		GameStatus::game_ended = true;
		return;
	}
	for (auto& move : all_possible_moves) {
		if (lower_piece_rep == 'n' || lower_piece_rep == 'k' || lower_piece_rep == 'p') {

			int int_piece_rep = static_cast<size_t>(piece);

			if (start_pos[0] == move.start_square[0] && start_pos[1] == move.start_square[1] && end_pos[0] == move.target_square[0] && end_pos[1] == move.target_square[1]) {
				// Record the move
				if ((int_piece_rep == 1 || int_piece_rep == 7) && (end_pos[0] == 0 || end_pos[0] == 7)) {
					if (option != '/0') {
						piece = promotePawn(piece, end_pos, option);
					}
					else {
						while (true) {
							std::cout << "Pawn promotion. Pick a piece(Q,R,B,N): ";
							std::cin >> option;
							if (tolower(option) == 'q' || tolower(option) == 'r' || tolower(option) == 'b' || tolower(option) == 'n') {
								piece = promotePawn(piece, end_pos, option);
								break;
							}
						}
					}

				}

				PieceType target_piece = board[end_pos[0]][end_pos[1]];
				Move move{ start_pos, end_pos, piece, target_piece };
				move_history.push_back(move);

				// Move the piece
				board[end_pos[0]][end_pos[1]] = piece;
				board[start_pos[0]][start_pos[1]] = PieceType::EMPTY;
				counter++;
				break;
			}
		}
		else if (lower_piece_rep == 'b' || lower_piece_rep == 'r' || lower_piece_rep == 'q') {
			std::vector<std::array<int, 2>> possible_moves = slidingAttacks(piece, start_pos);

			if (start_pos[0] == move.start_square[0] && start_pos[1] == move.start_square[1] && end_pos[0] == move.target_square[0] && end_pos[1] == move.target_square[1]) {
				PieceType target_piece = board[end_pos[0]][end_pos[1]];

				// Record the move
				Move move{ start_pos, end_pos, piece, target_piece };
				move_history.push_back(move);

				// Move the piece
				board[end_pos[0]][end_pos[1]] = piece;
				board[start_pos[0]][start_pos[1]] = PieceType::EMPTY;
				counter++;
				break;
			}
		}
	}
	
}
void ChessBoard::doMove(std::string move_pos) {
	std::array < std::array<int, 2>, 2> moveArray = convertMoveStringToArray(move_pos);
	std::array<int, 2> start_pos = moveArray[0];
	std::array<int, 2> end_pos = moveArray[1];
	char option = move_pos[4];
	PieceType piece = board[start_pos[0]][start_pos[1]];

	int int_piece_rep = static_cast<int>(piece);

	char piece_rep = pieces[static_cast<size_t>(piece)];
	char lower_piece_rep = tolower(piece_rep);

	PieceType target_piece = board[end_pos[0]][end_pos[1]];
	Move move{ start_pos, end_pos, piece, target_piece };
	move_history.push_back(move);

	board[end_pos[0]][end_pos[1]] = piece;
	board[start_pos[0]][start_pos[1]] = PieceType::EMPTY;
	counter++;
	}
void ChessBoard::undoMove() {
		if (!move_history.empty()) {
			Move last_move = move_history.back();
			move_history.pop_back();

			// Restore the piece positions
			board[last_move.start_square[0]][last_move.start_square[1]] = last_move.moved_piece;
			board[last_move.target_square[0]][last_move.target_square[1]] = last_move.captured_piece;

			//GameStatus::turn == 1 ? GameStatus::turn = 2 : GameStatus::turn = 1;
			counter--;
		}
}
bool ChessBoard::validateMoves(std::array<int, 2> next_pos) {
	return next_pos[0] >= 0 && next_pos[0] < 8 && next_pos[1] >= 0 && next_pos[1] < 8;
}

PieceType ChessBoard::promotePawn(PieceType piece, std::array<int, 2> end_pos, char option) {
		int int_piece_rep = static_cast<size_t>(piece);

		PieceType p{};
		if (int_piece_rep >= 7) {
			switch (tolower(option)) {
			case 'r':
				p = PieceType::BLACK_ROOK;
				break;
			case 'n':
				p = PieceType::BLACK_KNIGHT;
				break;
			case 'b':
				p = PieceType::BLACK_BISHOP;
				break;
			case 'q':
				p = PieceType::BLACK_QUEEN;
				break;

			}
		}
		else if (int_piece_rep < 7 && int_piece_rep != 0) {
			switch (tolower(option)) {
			case 'r':
				p = PieceType::WHITE_ROOK;
				break;
			case 'n':
				p = PieceType::WHITE_KNIGHT;
				break;
			case 'b':
				p = PieceType::WHITE_BISHOP;
				break;
			case 'q':
				p = PieceType::WHITE_QUEEN;
				break;
			}
		}

		return p;
	
}

void ChessBoard::addToEnPassantList(std::array<int,2>& offset, std::array<int,2>& offset_dist, PieceType piece_at_next_pos, char pawnTypeToCheckAgainst, int onFirstBreak) {
	if (offset[0] == onFirstBreak && pieces[static_cast<size_t>(piece_at_next_pos)] == pieces[0]) {
		std::array<int, 2> offset_dist_left = { offset_dist[0], offset_dist[1] - 1 };
		std::array<int, 2> offset_dist_right = { offset_dist[0], offset_dist[1] + 1 };

		if (validateMoves(offset_dist_left) && pieces[static_cast<size_t>(piece_at_next_pos)] == pieces[0]) {
			PieceType piece_left = board[offset_dist_left[0]][offset_dist_left[1]];
			char rep_left = pieces[static_cast<size_t>(piece_left)];
			if (rep_left == pawnTypeToCheckAgainst) en_passant.push_back(offset_dist_left);
		}
		if (validateMoves(offset_dist_right) && pieces[static_cast<size_t>(piece_at_next_pos)] == pieces[0]) {
			PieceType piece_right = board[offset_dist_right[0]][offset_dist_right[1]];
			char rep_right = pieces[static_cast<size_t>(piece_right)];
			if (rep_right == pawnTypeToCheckAgainst) en_passant.push_back(offset_dist_left);
		}

	}
}

void ChessBoard::resolveEnPassantMoves(std::array<int,2>& start_pos) {
	for (auto& pos : en_passant) {
		if (start_pos[0] == pos[0] && start_pos[1] == pos[1]) {
			std::array<int, 2> offset_dist_left = { pos[0], pos[1] - 1 };
			std::array<int, 2> offset_dist_right = { pos[0], pos[1] + 1 };

			if (validateMoves(offset_dist_left)) {
				PieceType piece_left = board[offset_dist_left[0]][offset_dist_left[1]];
				char rep_left = pieces[static_cast<size_t>(piece_left)];
				if (rep_left == 'P') en_passant.push_back(offset_dist_left);
			}
			if (validateMoves(offset_dist_left)) {
				PieceType piece_right = board[offset_dist_right[0]][offset_dist_right[1]];
				char rep_right = pieces[static_cast<size_t>(piece_right)];
				if (rep_right == 'P') en_passant.push_back(offset_dist_left);
			}
		}

	}
}

std::vector<std::array<int, 2>> ChessBoard::positionsStartOfGame(std::array<std::array<int,2>,2>& start_game_offset,
	std::array<int,2>& start_pos, int onFirstBreak, char pawnTypeToCheckAgainst) {

	std::vector < std::array<int, 2>> target_locations(2);
	target_locations.shrink_to_fit();
	for (auto& offset : start_game_offset) {

		
		std::array<int, 2> offset_dist = { start_pos[0] + offset[0], start_pos[1] + offset[1] };
		if (!validateMoves(offset_dist)) continue;
		
		PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
		if (offset_dist[0] == onFirstBreak && isalpha(pieces[static_cast<int>(piece_at_next_pos)])) break;
		if (islower(pieces[static_cast<size_t>(piece_at_next_pos)])) {
				continue;
			
		if (pieces[static_cast<size_t>(piece_at_next_pos)] == pieces[0]) {
				target_locations.push_back(offset_dist);
			}


			//adding enpassant squares
			addToEnPassantList(offset, offset_dist, piece_at_next_pos, pawnTypeToCheckAgainst, onFirstBreak);


		}
	}


	return target_locations;
}
std::vector<std::array<int, 2>> ChessBoard::pawnMoves(PieceType piece, std::array<int, 2> start_pos) {
	std::vector<std::array<int, 2>> target_locations;

	if (islower(pieces[static_cast<size_t>(piece)])) {
		std::array<std::array<int, 2>, 2> black_piece_start_game_offset{
			{
				 { 1, 0},
				 { 2, 0 }

			}
		};

		std::array<int, 2> black_piece_offsets{
			{ 1, 0 }
		};

		std::array<std::array<int, 2>, 2> black_diag_offsets{
			{
				 { 1, -1},
				 { 1, 1 }
			}
		};

		for (auto& diag : black_diag_offsets) {
			std::array<int, 2> offset_dist = { start_pos[0] + diag[0], start_pos[1] + diag[1] };
			if (!validateMoves(offset_dist)) continue;

			PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
			if (isupper(pieces[static_cast<size_t>(piece_at_next_pos)])) {
				target_locations.push_back(offset_dist);
			}
		}

		if (start_pos[0] == 1) {
			std::vector<std::array<int,2>> start_of_game_offsets = positionsStartOfGame(black_piece_start_game_offset, start_pos, 2, 'P');
		
			for (auto& offset : start_of_game_offsets) {
				target_locations.push_back(offset);
			}
		}

		if (start_pos[0] != 1) {
			std::array<int, 2> offset = black_piece_offsets;
			std::array<int, 2> offset_dist = { start_pos[0] + offset[0], start_pos[1] + offset[1] };

			//checking if we are on an enpassant square
			if (!en_passant.empty()) {
				resolveEnPassantMoves(start_pos);

			}
			if (validateMoves(offset_dist)) {
				PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
				if (pieces[static_cast<size_t>(piece_at_next_pos)] == pieces[0]) {
					target_locations.push_back(offset_dist);
				}
			}
			
		}


		
		
	}
	else if (isupper(pieces[static_cast<size_t>(piece)])) {
		std::array<std::array<int, 2>, 2> white_piece_start_game_offset{
			{
				{ -1, 0},
				{-2, 0}
			}
		};
		std::array<int,2> white_piece_offsets{
			
				
				{ -1, 0 }
			
		};
		std::array<std::array<int, 2>, 2> white_diag_offsets{
			{
				{ -1, 1},
				{ -1, -1 }
			}
		};
		for (auto& diag : white_diag_offsets) {
			std::array<int, 2> offset_dist = { start_pos[0] + diag[0], start_pos[1] + diag[1] };
			if (!validateMoves(offset_dist)) continue;

			

			PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
			if (islower(pieces[static_cast<size_t>(piece_at_next_pos)])) {
				target_locations.push_back(offset_dist);
			}
		}
		
		if (start_pos[0] == 6) {
			std::vector<std::array<int, 2>> start_of_game_offsets = positionsStartOfGame(white_piece_start_game_offset, start_pos, 5, 'p');

			for (auto& offset : start_of_game_offsets) {
				target_locations.push_back(offset);
			}
		}

		if (start_pos[0] != 6) {
			std::array<int, 2> offset = white_piece_offsets;
			std::array<int, 2> offset_dist = { start_pos[0] + offset[0], start_pos[1] + offset[1] };
			//checking if we are on an enpassant square
			if (!en_passant.empty()) {
				resolveEnPassantMoves(start_pos);
			}
			
			if (validateMoves( offset_dist )) {
				PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
				if (pieces[static_cast<size_t>(piece_at_next_pos)] == pieces[0]) {
					target_locations.push_back(offset_dist);
				}

			}
			

		}


	}
	return target_locations;
}

std::vector<std::array<int, 2>> ChessBoard::knightMoves(PieceType piece, std::array<int, 2> start_pos) {
	std::vector<std::array<int, 2>> target_locations;

	std::array<std::array<int, 2>, 8> knight_offsets{
			{
				{ -2, -1 }, { -2, 1},
				{ 2, 1 }, { 2, 1 },
				{ -1, -2 }, { -1, 2},
				{1, -2}, {1, 2}
			}
	};

	for (auto& offset : knight_offsets) {
		std::array<int, 2> offset_dist = { start_pos[0] + offset[0], start_pos[1] + offset[1] };
		if (!validateMoves(offset_dist)) {
			continue;
		}
		PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
		if (static_cast<size_t>(piece_at_next_pos) >= 7 && static_cast<size_t>(piece) == 3) {
			target_locations.push_back(offset_dist);
			continue;
		}
		else if (static_cast<size_t>(piece_at_next_pos) < 7 && static_cast<size_t>(piece) == 9) {
			target_locations.push_back(offset_dist);
			continue;
		}
		else if (static_cast<size_t>(piece_at_next_pos) >= 7 && static_cast<size_t>(piece_at_next_pos) <= 12 && static_cast<size_t>(piece) == 9) {
			continue;
		}
		else if (static_cast<size_t>(piece_at_next_pos) >= 1 && static_cast<size_t>(piece_at_next_pos) < 7 && static_cast<size_t>(piece) == 3) {
			continue;
		}
		target_locations.push_back(offset_dist);
	}
	return target_locations;
}


std::vector<std::array<int, 2>> ChessBoard::kingMoves(PieceType piece, std::array<int, 2> start_pos) {
	std::vector<std::array<int, 2>> target_locations;
	if (islower(pieces[static_cast<size_t>(piece)])) {
		std::array<std::array<int, 2>, 4> black_piece_offsets{
	{
		 { -1, 0},
		 { 1, 0 },
		 { 1, -1},
		 {1 ,   1}

	}
		};
		for (auto& offset : black_piece_offsets) {
			std::array<int, 2> offset_dist = { start_pos[0] + offset[0], start_pos[1] + offset[1] };
			if (!validateMoves(offset_dist)) {
				continue;
			}

			PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
			if (islower(pieces[static_cast<size_t>(piece_at_next_pos)])) {
				continue;
			}
			target_locations.push_back(offset_dist);
		}
	}
	else if (isupper(pieces[static_cast<size_t>(piece)])) {
		std::array<std::array<int, 2>, 4> white_piece_offsets{
	{
		 { -1, 0},
		 { 1, 0 },
		 { 1, -1},
		 {1 ,   1}

	}
		};
		for (auto& offset : white_piece_offsets) {
			std::array<int, 2> offset_dist = { start_pos[0] + offset[0], start_pos[1] + offset[1] };
			
			if (!validateMoves(offset_dist)) {
				continue;
			}
			PieceType piece_at_next_pos = board[offset_dist[0]][offset_dist[1]];
			if (isupper(pieces[static_cast<size_t>(piece_at_next_pos)])) {
				continue;
			}
			target_locations.push_back(offset_dist);
		}
	}
	return target_locations;
}
std::vector<std::array<int,2>> ChessBoard::leapingPieces(PieceType piece, std::array<int,2> start_pos) {


	if (tolower(pieces[static_cast<size_t>(piece)]) == 'n') {
		
		std::vector<std::array<int, 2>> knight_moves = knightMoves(piece, start_pos);
		return knight_moves;
	}

	else if (tolower(pieces[static_cast<size_t>(piece)]) == 'p') {

		std::vector<std::array<int, 2>> pawn_moves = pawnMoves(piece, start_pos);
		return pawn_moves;
	}

	else if (tolower(pieces[static_cast<size_t>(piece)]) == 'k') {
		std::vector<std::array<int, 2>> king_moves = kingMoves(piece, start_pos);
		return king_moves;
	}

		
	return {};
	
	
}


std::vector<std::array<int, 2>> ChessBoard::slidingAttacks(PieceType piece, std::array<int, 2> start_pos) {
	std::vector<std::array<int, 2>> target_pos;
	std::array<std::array<int, 2>, 8> offsets{
		{
			{-1, -1}, {-1, 1}, {1, -1}, {1, 1},
			{-1, 0}, {1, 0}, {0, -1}, {0, 1}
		}
	};

	int begin{}, end{};
	switch (tolower(pieces[static_cast<size_t>(piece)])) {
		case 'r':
			begin = 4;
			end = 7;
			break;
		case 'b':
			begin = 0;
			end = 3;
			break;
		case 'q':
			begin = 0;
			end = 7;
			break;
	}
	
	for (begin; begin <= end; begin++) {
		std::array<int, 2> offset = offsets[begin];
		std::array<int, 2> offset_pos{start_pos[0] + offset[0], start_pos[1] + offset[1]};
		while (validateMoves(offset_pos)) {
			PieceType piece_next_pos{board[offset_pos[0]][offset_pos[1]]};
			if (isalpha(pieces[static_cast<size_t>(piece_next_pos)])) {
				int next_pos_int_rep = static_cast<int>(piece_next_pos);
				int piece_int_rep = static_cast<int>(piece);
				char next_pos_char_rep = static_cast<char>(pieces[static_cast<size_t>(piece_next_pos)]);

				if (piece_int_rep < 7 && islower(next_pos_char_rep)) {
					target_pos.push_back(offset_pos);
				}
				else if (piece_int_rep >= 7 && isupper(next_pos_char_rep)) {
					target_pos.push_back(offset_pos);
				}
				break;
			} 
			target_pos.push_back(offset_pos);
			offset_pos[0] += offset[0]; offset_pos[1] += offset[1];
		}
	}
	return target_pos;
}

std::vector<Move> ChessBoard::generateMoves() {
	std::vector<Move> moves;
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			PieceType piece = board[i][j];
			char rep = pieces[static_cast<size_t>(piece)];
			std::array<int, 2> start_pos = { i,j };

			if (getCurrentPlayer(counter) == 1 && isupper(rep)) {
				
				if (rep == 'P' || rep == 'N' || rep == 'K') {
					std::vector<std::array<int, 2>> leaping_attacks = leapingPieces(piece, start_pos);
					for (auto& attack_pos : leaping_attacks) {
						for (auto& pos : en_passant) {
							if (pos[0] == attack_pos[0] && pos[1] == attack_pos[1]) {
								PieceType captured = board[attack_pos[0] - 1][attack_pos[1]];
								Move mv = { start_pos, attack_pos, piece, captured };
								moves.push_back(mv);
								continue;
							}
						}
						PieceType captured = board[attack_pos[0]][attack_pos[1]];
						Move mv = { start_pos, attack_pos, piece, captured };
						moves.push_back(mv);
					}
				}
				else if (rep == 'R' || rep == 'B' || rep == 'Q') {
					std::vector<std::array<int, 2>> sliding_attacks = slidingAttacks(piece, start_pos);
					for (auto& attack_pos : sliding_attacks) {
						PieceType captured = board[attack_pos[0]][attack_pos[1]];
						Move mv = { start_pos, attack_pos, piece, captured };
						moves.push_back(mv);
					}
				}
			}
			else if (getCurrentPlayer(counter) == 2 && islower(rep)) {
				char lower_rep = tolower(rep);
				if (lower_rep == 'p' || lower_rep == 'n' || lower_rep == 'k') {
					std::vector<std::array<int, 2>> leaping_attacks = leapingPieces(piece, start_pos);
					for (auto& attack_pos : leaping_attacks) {
						for (auto& pos : en_passant) {
							if (pos[0] == attack_pos[0] && pos[1] == attack_pos[1]) {
								PieceType captured = board[attack_pos[0] - 1][attack_pos[1]];
								Move mv = { start_pos, attack_pos, piece, captured };
								moves.push_back(mv);
								continue;
							}
						}
						PieceType captured = board[attack_pos[0]][attack_pos[1]];
						Move mv = { start_pos, attack_pos, piece, captured };
						moves.push_back(mv);
					}
				}
				else if (lower_rep == 'r' || lower_rep == 'b' || lower_rep == 'q') {
					std::vector<std::array<int, 2>> sliding_attacks = slidingAttacks(piece, start_pos);
					for (auto& attack_pos : sliding_attacks) {
						PieceType captured = board[attack_pos[0]][attack_pos[1]];
						Move mv = { start_pos, attack_pos, piece, captured };
						moves.push_back(mv);
					}
				}
			}

		}
	}
	return moves;

}

std::vector<Move> ChessBoard::generateLegalMoves() {
	std::vector<Move> pseudoLegalMoves = generateMoves();
	std::vector<Move> legalMoves;

	for (auto& move_verify : pseudoLegalMoves) {
		bool king_in_check = false;
		std::string start_str = convertPosArraytoMoveString(move_verify.start_square);
		std::string end_str = convertPosArraytoMoveString(move_verify.target_square);
		std::string move_str = start_str + end_str;
		doMove(move_str);
		std::vector<Move> opponent_responses = generateMoves();
		for (auto& move : opponent_responses) {
			PieceType piece = board[move.target_square[0]][move.target_square[1]];
			if (piece == PieceType::WHITE_KING || piece == PieceType::BLACK_KING) {
				king_in_check = true;
				break;
			}
			
		}
		if (king_in_check) { undoMove(); continue; }
		legalMoves.push_back(move_verify);
		undoMove();
		
	}
	return legalMoves;
}