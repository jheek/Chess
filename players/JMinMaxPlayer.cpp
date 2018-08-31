//
// Created by Lukas Bos on 13/12/2017.
//

#include <iostream>
#include "JMinMaxPlayer.h"

using namespace sf;
using namespace std;

JMinMaxPlayer::JMinMaxPlayer(PieceColor color, int moveBudget)
    : Player(color), moveBudget(moveBudget) {
  type = "JMinMaxPlayer - " + to_string(moveBudget);
  isHuman = false;
  leftoverBudget = 0;
}

Move *JMinMaxPlayer::getNextMove(Board *board) {
  MinMaxResult result = getMinMaxBoardScore(board, color, moveBudget + leftoverBudget, 0, INT_MIN, INT_MAX);
  leftoverBudget += moveBudget - result.usedBudget;

  if (result.bestMove == nullptr) {
    return nullptr;
  }
  result.bestMove->setSimulated(false);
  // result.bestMove->generateName();
  // cout << "best move: " << result.bestMove->getName() << endl;
  cout << "advantage for white: " << getBoardScore(board, PieceColor::WHITE) << endl;
  cout << "max search depth: " << result.maxDepth << endl;
  cout << "used budget: " << result.usedBudget << endl;

  return result.bestMove;
}

bool moveCompareMax(Move* a, Move* b) {
  return a->getScoreAtEnd() > b->getScoreAtEnd();
}

bool moveCompareMin(Move* a, Move* b) {
  return a->getScoreAtEnd() < b->getScoreAtEnd();
}

MinMaxResult::MinMaxResult() {
  bestScore = 0;
  bestMove = nullptr;
  maxDepth = 0;
  usedBudget = 0;
}

MinMaxResult JMinMaxPlayer::getMinMaxBoardScore(Board* board, PieceColor colorToMove, int budget, int depth, int alpha, int beta) {
  MinMaxResult result;
  bool isMaximizing = colorToMove == color;
  result.bestScore = isMaximizing ? INT_MIN : INT_MAX;

  vector<ChessPiece *> pieces = board->getPiecesByColor(colorToMove);
  vector<Move*> moves;
  for (ChessPiece *piece : pieces) {
    vector<Move*> pieceMoves = piece->getAvailableMoves(depth == 0);
    for (auto move : pieceMoves) {
      move->setSimulated(true);
      board->doMove(move);
      int score = getBoardScore(board, color);
      move->setScoreAtEnd(score);
      board->undoMove();
      moves.push_back(move);
    }
  }
  if (isMaximizing) {
    sort(moves.begin(), moves.end(), moveCompareMax);
  } else {
    sort(moves.begin(), moves.end(), moveCompareMin);
  }

  int numMoves = moves.size();
  int remainingMoves = numMoves;
  result.usedBudget = numMoves;
  for (auto move : moves) {
      int subBudget = min(budget, max(budget - result.usedBudget, 1) / remainingMoves);
      MinMaxResult subResult;
      if (subBudget <= 1) {
        subResult.bestMove = move;
        subResult.bestScore = move->getScoreAtEnd();
        subResult.maxDepth = depth;
      } else {
        board->doMove(move);
        subResult = getMinMaxBoardScore(board, inverse(colorToMove), subBudget, depth + 1, alpha, beta);
        board->undoMove();
      }
      
      result.usedBudget += subResult.usedBudget + 1;
      remainingMoves--;
      if (isMaximizing && subResult.bestScore > result.bestScore) {
        result.bestScore = subResult.bestScore;
        result.bestMove = move;
        alpha = max(result.bestScore, alpha);
        result.maxDepth = subResult.maxDepth;
      } else if (!isMaximizing && subResult.bestScore < result.bestScore) {
        result.bestScore = subResult.bestScore;
        result.bestMove = move;
        beta = min(result.bestScore, beta);
        result.maxDepth = subResult.maxDepth;
      }
      if (beta <= alpha) {
        break;
      }
  }

  return result;
}


tuple<int, int, int> JMinMaxPlayer::getMoveScore(Board *board, Move *move, int budget, int depth, int alpha, int beta) {
  // PieceColor colorToMove = inverse(move->getInitialPiece()->getColor());

  // if (budget <= 1) {
  //   // move->setSimulated(true);
  //   // board->doMove(move);
  //   // int boardScore = getBoardScore(board, color);
  //   // board->undoMove();
  //   int boardScore = move->getScoreAtEnd();
  //   return {boardScore, 0, depth};
  // }

  // int bestScore;
  // bool isMaximizing = colorToMove == color;
  // bestScore = isMaximizing ? INT_MIN : INT_MAX;

  // move->setSimulated(true);
  // board->doMove(move);

  // int maxDepth = 0;

  // vector<ChessPiece *> pieces = board->getPiecesByColor(colorToMove);
  // vector<Move*> moves;
  // for (ChessPiece *piece : pieces) {
  //   vector<Move*> pieceMoves = piece->getAvailableMoves(true);
  //   for (auto move : pieceMoves) {
  //     move->setSimulated(true);
  //     board->doMove(move);
  //     int score = getBoardScore(board, color);
  //     move->setScoreAtEnd(score);
  //     board->undoMove();
  //     moves.push_back(move);
  //   }
  // }
  // if (isMaximizing) {
  //   sort(moves.begin(), moves.end(), moveCompareMax);
  // } else {
  //   sort(moves.begin(), moves.end(), moveCompareMin);
  // }
  // int numMoves = moves.size();
  // int remainingMoves = numMoves;
  // int usedBudget = numMoves;
  // for (auto move : moves) {
  //     int boost = 1;
  //     // if (remainingMoves * 2 > numMoves)
  //     //   boost = 2;

  //     int subBudget = min(budget, max(budget - usedBudget, 1) * boost / remainingMoves);
  //     auto [newScore, subUsedBudget, newDepth] = getMoveScore(board, move, subBudget, depth + 1, alpha, beta);
  //     usedBudget += subUsedBudget + 1;
  //     remainingMoves--;

  //     if (isMaximizing && newScore > bestScore) {
  //       bestScore = newScore;
  //       alpha = max(bestScore, alpha);
  //       maxDepth = newDepth;
  //     } else if (!isMaximizing && newScore < bestScore) {
  //       bestScore = newScore;
  //       beta = min(bestScore, beta);
  //       maxDepth = newDepth;
  //     }
  //     if (beta <= alpha) {
  //       // cout << "pruned branch!" << endl;
  //       break;
  //     }
  // }
  
  // board->undoMove();

  // return {bestScore, usedBudget, maxDepth};
  return {0, 0, 0};
}

int JMinMaxPlayer::getBoardScore(Board *board, PieceColor c) {
  vector<ChessPiece *> friendlyPieces = board->getPiecesByColor(c);
  vector<ChessPiece *> opponentPieces = board->getPiecesByColor(inverse(c));

  long score = evaluateScore(board, friendlyPieces) - evaluateScore(board, opponentPieces);
  return score;
}

int JMinMaxPlayer::evaluateScore(Board *board, vector<ChessPiece *> pieces) {
  int materialScore = 0;
  int locationScore = 0;
  int movementScore = 0;

  for (ChessPiece *piece : pieces) {
    materialScore += getPieceScore(piece);
    locationScore += getLocationScore(board, piece, piece->getLocation()->getCoordinates());
    movementScore += piece->getAvailableSquares(false).size();
  }

  if (materialScore < 9900) phase = MIDDLEGAME;
  if (materialScore < 9000) phase = ENDGAME;

  int materialFactor = 10;
  int locationFactor = phase==OPENING ? 1 : 0;
  int movementFactor = phase==ENDGAME ? 2 : 1;

  long total = materialScore*materialFactor
      + locationScore*locationFactor
      + movementScore*movementFactor;
  return total;
}

int JMinMaxPlayer::getLocationScore(Board *board, ChessPiece *piece, Vector2i location) {
  // source: https://chessprogramming.wikispaces.com/Simplified+evaluation+function

  PieceType type = piece->getType();
  array<array<int, 8>, 8> locationScores;

  switch (type) {
    case KING:
      locationScores = {{
                            {{-30, -40, -40, -50, -50, -40, -40, -30}},
                            {{-30, -40, -40, -50, -50, -40, -40, -30}},
                            {{-30, -40, -40, -50, -50, -40, -40, -30}},
                            {{-30, -40, -40, -50, -50, -40, -40, -30}},
                            {{-20, -30, -30, -40, -40, -30, -30, -20}},
                            {{-10, -20, -20, -20, -20, -20, -20, -10}},
                            {{20, 20, 0, 0, 0, 0, 20, 20}},
                            {{20, 30, 10, 0, 0, 10, 30, 20}}}};
      break;
    case QUEEN:
      locationScores = {{
                            {{-20, -10, -10, -5, -5, -10, -10, -20}},
                            {{-10, 0, 0, 0, 0, 0, 0, -10}},
                            {{-10, 0, 5, 5, 5, 5, 0, -10}},
                            {{-5, 0, 5, 5, 5, 5, 0, -5}},
                            {{0, 0, 5, 5, 5, 5, 0, -5}},
                            {{-10, 5, 5, 5, 5, 5, 0, -10}},
                            {{-10, 0, 5, 0, 0, 0, 0, -10}},
                            {{-20, -10, -10, -5, -5, -10, -10, -20}}
                        }};
      break;
    case ROOK:
      locationScores = {{
                            {{0, 0, 0, 0, 0, 0, 0, 0}},
                            {{5, 10, 10, 10, 10, 10, 10, 5}},
                            {{-5, 0, 0, 0, 0, 0, 0, -5}},
                            {{-5, 0, 0, 0, 0, 0, 0, -5}},
                            {{-5, 0, 0, 0, 0, 0, 0, -5}},
                            {{-5, 0, 0, 0, 0, 0, 0, -5}},
                            {{-5, 0, 0, 0, 0, 0, 0, -5}},
                            {{0, 0, 0, 5, 5, 0, 0, 0}}
                        }};
      break;
    case KNIGHT:
      locationScores = {{
                            {{-50, -40, -30, -30, -30, -30, -40, -50}},
                            {{-40, -20, 0, 0, 0, 0, -20, -40}},
                            {{-30, 0, 10, 15, 15, 10, 0, -30}},
                            {{-30, 5, 15, 20, 20, 15, 5, -30}},
                            {{-30, 0, 15, 20, 20, 15, 0, -30}},
                            {{-30, 5, 10, 15, 15, 10, 5, -30}},
                            {{-40, -20, 0, 5, 5, 0, -20, -40}},
                            {{-50, -40, -30, -30, -30, -30, -40, -50}}
                        }};
      break;
    case BISHOP:
      locationScores = {{
                            {{-20, -10, -10, -10, -10, -10, -10, -20}},
                            {{-10, 0, 0, 0, 0, 0, 0, -10}},
                            {{-10, 0, 5, 10, 10, 5, 0, -10}},
                            {{-10, 5, 5, 10, 10, 5, 5, -10}},
                            {{-10, 0, 10, 10, 10, 10, 0, -10}},
                            {{-10, 10, 10, 10, 10, 10, 10, -10}},
                            {{-10, 5, 0, 0, 0, 0, 5, -10}},
                            {{-20, -10, -10, -10, -10, -10, -10, -20}}
                        }};
      break;
    case PAWN:
      locationScores = {{
                            {{0, 0, 0, 0, 0, 0, 0, 0}},
                            {{50, 50, 50, 50, 50, 50, 50, 50}},
                            {{10, 10, 20, 30, 30, 20, 10, 10}},
                            {{5, 5, 10, 25, 25, 10, 5, 5}},
                            {{0, 0, 0, 20, 20, 0, 0, 0}},
                            {{5, -5, -10, 0, 0, -10, -5, 5}},
                            {{5, 10, 10, -20, -20, 10, 10, 5}},
                            {{0, 0, 0, 0, 0, 0, 0, 0}}
                        }};
  }

  int y = location.y;
  int x = location.x;
  if (board->getBottomPlayer()->getColor()!=piece->getColor()) {
    y = 7 - location.y;
    x = 7 - location.x;
  }
  string color = piece->getColor()==PieceColor::BLACK ? "Black" : "White";
  string coordinates = "[" + to_string(piece->getLocation()->getCoordinates().x) + ","
      + to_string(piece->getLocation()->getCoordinates().y) + "]";
  // cout << piece->getName() << "\t " <<  color << "\t " << coordinates << " \t " << locationScores[y][x] << endl;

  return locationScores[y][x];
}
int JMinMaxPlayer::getPieceScore(ChessPiece *piece) {
  switch (piece->getType()) {
    case KING: return 10000;
    case QUEEN: return 900;
    case ROOK: return 500;
    case BISHOP: return 330;
    case KNIGHT: return 320;
    case PAWN: return 100;
  }
  return 0;
}
