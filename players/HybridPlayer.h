//
// Created by Lukas Bos on 31/08/2018.
// This class combines MinMaxPlayer with MonteCarloPlayer
//

#ifndef CHESS_HYBRIDPLAYER_H
#define CHESS_HYBRIDPLAYER_H

#include "../Player.h"
class HybridPlayer : public Player {
 public:
  HybridPlayer(PieceColor color, int depth);
  Move *getNextMove(Board *board);
 private:
  int playout(Board *board, PieceColor colorToMove, int counter);
  int getMoveScore(Board *board, Move *move, int exit, int alpha, int beta);
  int getBoardScore(Board *board, PieceColor c);
  int evaluateScore(Board *board, vector<ChessPiece *> pieces);
  int getLocationScore(Board *board, ChessPiece *piece, Vector2i location);
  int getPieceScore(ChessPiece *piece);
  int depth = 0;

  enum GamePhase {
    OPENING, MIDDLEGAME, ENDGAME
  };

  GamePhase phase = OPENING;
};

#endif //CHESS_HYBRIDPLAYER_H
