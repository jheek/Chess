//
// Created by Lukas Bos on 11/12/2017.
//

#include "RandomPlayer.h"

using namespace sf;

RandomPlayer::RandomPlayer() {
    type = "Randomplayer";
}

RandomPlayer::RandomPlayer(PieceColor color) : Player(color) {
    type = "Randomplayer";
}

Move *RandomPlayer::getNextMove(Board * board) {
    std::vector<ChessPiece *> pieces = board->getPiecesByColor(color);

    for (int i = 0; i < pieces.size(); i++) {
        ChessPiece *randomPiece = pieces[std::rand() % pieces.size()];
        std::vector<Square *> moves = randomPiece->getAvailableSquares(true);

        if (!moves.empty()) { // if the chesspiece has available moves
            return new Move(board, randomPiece->location, moves.at(std::rand() % moves.size())); // return random move
        }
    }

    return NULL;
}
