//
// Created by Lukas Bos on 11/12/2017.
//

#include "Move.h"

Move::Move() = default;

Move::Move(Board *board, Square *startOfMove, Square *endOfMove)
    : board(board), startOfMove(startOfMove), endOfMove(endOfMove) {
  identifyPieces();
  generateName();
}

Move::Move(Board *board, Square *startOfMove, Square *endOfMove, bool simulated)
    : board(board), startOfMove(startOfMove), endOfMove(endOfMove), simulated(simulated) {
  identifyPieces();
  if (!isSimulated()) generateName();
}

Move::Move(Board *board,
           Square *startOfMove,
           Square *endOfMove,
           bool simulated,
           CastlingSide castlingSide,
           ChessPiece *castlingRook,
           Square *rookTargetSquare,
           Square *initalRookSquare)
    : board(board),
      startOfMove(startOfMove),
      endOfMove(endOfMove),
      simulated(simulated),
      castlingSide(castlingSide),
      castlingRook(castlingRook),
      rookTargetSquare(rookTargetSquare),
      initalRookSquare(initalRookSquare) {
  identifyPieces();
  if (!isSimulated()) generateName();
}

// Simulate move to look if this causes a check(mate).
void Move::generateName() {
  if (castlingRook) {
    name = castlingSide==CastlingSide::QUEENSIDE ? "0-0-0" : "0-0";
    return;
  }
  char shortNames[] = "KQRBNP";
  name = shortNames[initialPiece->getType()];
  if (name=="P") name = ""; // pawn is usually not written down.
  if (this->getTakenPiece()) name += 'x';
  name += endOfMove->getName();

  bool tempIsSimulated = simulated;
  simulated = true;
  board->doMove(this);
  if (board->checkMate()) {
    name += '#';
  } else if (board->isInCheck(inverse(initialPiece->getColor()))) {
    name += '+';
  }
  board->undoMove();
  simulated = tempIsSimulated;
}

void Move::identifyPieces() {
  initialPiece = getStartOfMove()->getChessPiece();
  takenPiece = getEndOfMove()->getChessPiece();
}

Square *Move::getStartOfMove() const {
  return startOfMove;
}

Square *Move::getEndOfMove() const {
  return endOfMove;
}

ChessPiece *Move::getInitialPiece() const {
  return initialPiece;
}

ChessPiece *Move::getTakenPiece() const {
  return takenPiece;
}

bool Move::isSimulated() const {
  return simulated;
}

void Move::setSimulated(bool simulated) {
  Move::simulated = simulated;
}

const string &Move::getName() const {
  return name;
}

ChessPiece *Move::getCastlingRook() const {
  return castlingRook;
}

Square *Move::getRookTargetSquare() const {
  return rookTargetSquare;
}

Square *Move::getInitalRookSquare() const {
  return initalRookSquare;
}

bool Move::isPromoting() const {
  return promoting;
}

void Move::setPromoting(bool promoting) {
  Move::promoting = promoting;
}

ChessPiece *Move::getEnPassantTakenPiece() const {
  return enPassantTakenPiece;
}

void Move::setEnPassantTakenPiece(ChessPiece *enPassantTakenPiece) {
  Move::enPassantTakenPiece = enPassantTakenPiece;
}

int Move::getScoreAtEnd() const {
  return scoreAtEnd;
}

void Move::setScoreAtEnd(int scoreAtEnd) {
  Move::scoreAtEnd = scoreAtEnd;
}
