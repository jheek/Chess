//
// Created by Lukas Bos on 13/12/2017.
//

#ifndef CHESS_JMINMAXPLAYER_H
#define CHESS_JMINMAXPLAYER_H

#include "../constants.h"
#include "../Board.h"
#include "../Player.h"
#include "../PieceColor.h"

using namespace std;

class MinMaxResult {
  public:
    MinMaxResult();
    int bestScore;
    Move *bestMove;
    int maxDepth;
    int usedBudget;
};

class JMinMaxPlayer : public Player {
 public:
  explicit JMinMaxPlayer(PieceColor color);
  explicit JMinMaxPlayer(PieceColor color, int moveBudget);

  Move *getNextMove(Board *board) override;

  tuple<int, int, int> getMoveScore(Board *, Move *, int, int, int, int);
  MinMaxResult getMinMaxBoardScore(Board* board, PieceColor colorToMove, int budget, int depth, int alpha, int beta);
 private:
  int moveBudget;
  int leftoverBudget;
  int getBoardScore(Board *, PieceColor);
  int evaluateScore(Board *, vector<ChessPiece *>);

  enum GamePhase {
    OPENING, MIDDLEGAME, ENDGAME
  };

  GamePhase phase = OPENING;
  int getLocationScore(Board *board, ChessPiece *piece, Vector2i location);
  int getPieceScore(ChessPiece *piece);
};



#endif //CHESS_JMINMAXPLAYER_H
