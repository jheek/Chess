//
// Created by Lukas Bos on 31/08/2018.
//

#include "MonteCarloPlayer.h"

MonteCarloPlayer::MonteCarloPlayer(PieceColor color) : Player(color) {
  type = "MonteCarloPlayer";
  isHuman = false;
}

Move *MonteCarloPlayer::getNextMove(Board *board) {
  double bestMoveScore = INT_MIN;
  for (ChessPiece *piece : board->getPiecesByColor(color)) {
    for (Move *move : piece->getAvailableMoves(true)) {
      // for every move we do a random playout

      long scoreSum = 0;
      int numOfGamesPerMove = 100;
      move->setSimulated(true);
      board->doMove(move);
      for (int i = 0; i < numOfGamesPerMove; i++) {
        scoreSum += playout(board, color, 0);
      }
      double score = scoreSum/numOfGamesPerMove;
      if (score > bestMoveScore) {
        bestMoveScore = score;
        nextMove = move;
      }
      board->undoMove();
    }
  }

  nextMove->setSimulated(false);
  return nextMove;
}

int MonteCarloPlayer::playout(Board *board, PieceColor colorToMove, int counter) {
  std::vector<ChessPiece *> pieces = board->getPiecesByColor(colorToMove);

  ChessPiece *randomPiece = pieces[rand()%pieces.size()];
  std::vector<Move *> moves = randomPiece->getAvailableMoves(true);
  if (!moves.empty() && counter < 1000) { // if the chesspiece has available moves
    Move *moveToTry = moves.at(rand()%moves.size());
    moveToTry->setSimulated(true);
    board->doMove(moveToTry);
    playout(board, inverse(colorToMove), counter + 1);
    board->undoMove();
  } else {

    // return a value of how favorable the score is for the initial player
    return getBoardScore(board, color);
  }
}

int MonteCarloPlayer::getBoardScore(Board *board, PieceColor c) {
  vector<ChessPiece *> friendlyPieces = board->getPiecesByColor(c);
  vector<ChessPiece *> opponentPieces = board->getPiecesByColor(inverse(c));

  long score = evaluateScore(board, friendlyPieces) - evaluateScore(board, opponentPieces);
  return score;
}

int MonteCarloPlayer::evaluateScore(Board *board, vector<ChessPiece *> pieces) {
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

int MonteCarloPlayer::getLocationScore(Board *board, ChessPiece *piece, Vector2i location) {
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
int MonteCarloPlayer::getPieceScore(ChessPiece *piece) {
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
