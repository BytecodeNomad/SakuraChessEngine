#include <iostream>
#include "ChessBoard.h"
#include <random>

#include <sstream>
#include "ChessBoard.h" 

void parsePositionCommand(const std::string& command);
void parseGoCommand();
void parseUciCommand();
void sendBestMove(std::string& bestMove);

int MoveGenerationTest(int depth) {
    if (depth == 0) {
        return 1;
    }
    std::vector<Move> moves = ChessBoard::generateLegalMoves();
    int num_pos = 0;

    for (auto& move : moves) {
        std::string move_str = ChessBoard::convertPosArraytoMoveString(move.start_square) + ChessBoard::convertPosArraytoMoveString(move.target_square);
        ChessBoard::doMove(move_str);
        num_pos += MoveGenerationTest(depth - 1);
        ChessBoard::undoMove();


    }
    return num_pos;
}

int main() {
    ChessBoard::initialize("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    //ChessBoard::initialize("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R");

    ChessBoard::printBoard();
    for (int i = 1; i <= 6; i++) {
        std::cout << "Depth: " << i << " Result: " << MoveGenerationTest(i) << " positions" << std::endl;
    }

    /*while (!GameStatus::game_ended) {

        std::string move;
        std::cin >> move;
        ChessBoard::makeMove(move);
        
        std::cin >> move;
        //sendBestMove(move_str);
        ChessBoard::makeMove(move);
        ChessBoard::printBoard();

    }*/
    
}


/*int main() {
   while (true) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "uci") {
            parseUciCommand();
        }
        else if (input == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (input.rfind("position") == 0) {
            //std::istringstream iss;

            parsePositionCommand(input);
        } 
        else if (input == "ucinewgame") {
            parsePositionCommand("startpos");
        }
        else if (input.rfind("go") == 0) {
            parseGoCommand();
        } 
        else if (input.rfind("info") == 0) {
            continue;
        }
        else if (input == "quit") {
            return 0;
        }
     }
     return 0;
 }
*/
 void parseUciCommand() {
     std::cout << "id name Sakura Engine" << std::endl;
    std::cout << "id author Charles" << std::endl;
    std::cout << "uciok" << std::endl;

 }

 void parsePositionCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string token;
    iss >> token; // 'position'
    iss >> token; // 'startpos' or 'fen'

     if (token == "startpos") {
         ChessBoard::initialize("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
         if (iss >> token) { // If there's more, it's 'moves
             if (token == "moves") {
                 while (iss >> token) {
                     ChessBoard::makeMove(token);
                 }
             }
         }
     } else if (token == "fen") {
         std::string fen;
         while (iss >> token && token != "moves") {
             fen += token + " ";
         }
         ChessBoard::initialize(fen);
         if (token == "moves") {
             while (iss >> token) {
                 ChessBoard::makeMove(token);
                 break;
             }
         }
     }
 }

 void parseGoCommand() {
     bool keepThinking = true;
     std::default_random_engine engine{ static_cast<unsigned int>(time(0)) };
     std::vector<Move> moves = ChessBoard::generateLegalMoves();
     std::uniform_int_distribution<unsigned int> randomInt{ 0, static_cast<unsigned int>(moves.size() - 1) };

     Move mv = moves[randomInt(engine)];

     std::string start_str = ChessBoard::convertPosArraytoMoveString(mv.start_square);
     std::string end_str = ChessBoard::convertPosArraytoMoveString(mv.target_square);
     std::string move_str = start_str + end_str;

     if (mv.target_square[0] == 7 || mv.target_square[0] == 0) {
         move_str += 'n';
     }
     sendBestMove( move_str );
     ChessBoard::makeMove(move_str);

 }

 void sendBestMove(std::string& bestMove) {
     //std::string bestMoveStr = parseMoveToString(bestMove);
     std::cout << "bestmove " << bestMove << std::endl;
 }
 



