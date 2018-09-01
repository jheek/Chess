//
// Created by Lukas Bos on 31/08/2018.
//

#ifndef CHESS_MONTECARLOPLAYER_H
#define CHESS_MONTECARLOPLAYER_H

#include "../Player.h"
class MonteCarloPlayer : public Player {
 public:
  MonteCarloPlayer(PieceColor color);
  Move *getNextMove(Board *board);

 private:
  int playout(Board *board, PieceColor colorToMove, int counter);
  int getBoardScore(Board *board, PieceColor c);
  int evaluateScore(Board *board, vector<ChessPiece *> pieces);
  int getLocationScore(Board *board, ChessPiece *piece, Vector2i location);
  int getPieceScore(ChessPiece *piece);

  enum GamePhase {
    OPENING, MIDDLEGAME, ENDGAME
  };

  GamePhase phase = OPENING;
};

#endif //CHESS_MONTECARLOPLAYER_H
