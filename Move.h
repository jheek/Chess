// Created by Lukas Bos on 11/12/2017.

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "Square.h"
#include "CastlingSide.h"
#include "ChessPiece.h"

using namespace std;

class Board;
class Square;
class Player;
class ChessPiece;
class Move {
 private:
  Board *board;
  Square *startOfMove = nullptr;
  Square *endOfMove = nullptr;
  ChessPiece *initialPiece = nullptr;
  ChessPiece *takenPiece = nullptr;
  Player *player;
  int scoreAtEnd;
  bool simulated = false;
  void identifyPieces();

  string name;

  // castling variables
  CastlingSide castlingSide;
  ChessPiece *castlingRook = nullptr;
  Square *rookTargetSquare = nullptr;
  Square *initalRookSquare = nullptr;

  // pawn promotion variables
  bool promoting = false;
  ChessPiece *promotedPiece = nullptr;

  // en passant variable
  ChessPiece *enPassantTakenPiece = nullptr;
 public:

  /// Initialize a move
  Move();

  /// Create a move
  Move(Board *, Square *, Square *);

  /// Create a move with or without simulation
  Move(Board *, Square *, Square *, bool);

  /// Create a castling move
  Move(Board *, Square *, Square *, bool, CastlingSide, ChessPiece *, Square *, Square *);

  /// Generate the chess notation name for the move
  void generateName();

  // Getters and setters
  Square *getStartOfMove() const;
  Square *getEndOfMove() const;
  ChessPiece *getInitialPiece() const;
  ChessPiece *getTakenPiece() const;
  bool isSimulated() const;
  void setSimulated(bool simulated);
  const string &getName() const;
  ChessPiece *getCastlingRook() const;
  Square *getRookTargetSquare() const;
  Square *getInitalRookSquare() const;
  bool isPromoting() const;
  void setPromoting(bool promoting);
  ChessPiece *getEnPassantTakenPiece() const;
  void setEnPassantTakenPiece(ChessPiece *enPassantTakenPiece);
  int getScoreAtEnd() const;
  void setScoreAtEnd(int score);
};

#endif //CHESS_MOVE_H
