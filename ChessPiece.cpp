//
// Created by Lukas Bos on 30/11/2017.
//

#include <iostream>
#include "ChessPiece.h"

using namespace sf;
using namespace std;

ChessPiece::ChessPiece() {
  type = PAWN;
};

ChessPiece::ChessPiece(Board *board, Square *location, PieceColor color)
    : board(board), location(location), color(color) {
  type = PAWN;
}

vector<Move *> ChessPiece::getAvailableMoves(bool considerCheck) {
  vector<Move *> moves;
  for (Square *square : getAvailableSquares(considerCheck)) {
    moves.push_back(new Move(board, location, square));
  }
  return moves;
}

/*
 * This function takes an array of directional vectors (sf::vector21)
 * For example: (1,1) means up-right and (-1,0) means left.
 * It returns the available legal moves according to the board.
 * Considercheck is usually always true, but it is needed for the program to detect check(mate)
 */

vector<Square *>
ChessPiece::calculateMovesForDirections(Square *location, Vector2i directions[], Board *board,
                                        PieceColor color, short amountOfDirections, short maxAmountOfSteps,
                                        bool considerCheck) {

  vector<Square *> moves;
  moves.reserve(21); // a piece cannot have more than 21 moves

  int y = location->getCoordinates().y;
  int x = location->getCoordinates().x;

  for (int i = 0; i < amountOfDirections; i++) {
    for (int j = 1; j <= maxAmountOfSteps; j++) {
      int xLocation = x + (j*directions[i].x);
      int yLocation = y + (j*directions[i].y);
      bool elementExists = xLocation >= 0 && xLocation < 8 && yLocation >= 0 && yLocation < 8;
      if (elementExists) {
        Square *element = board->getSquare(xLocation, yLocation);
        if (element->getChessPiece() && element->getChessPiece()->color==color) {
          break;
        } else if (element->getChessPiece()) {
          moves.push_back(element);
          break;
        }
        moves.push_back(element);
      }
    }
  }

  if (considerCheck) {
    return removeMovesLeadingToSelfCheck(moves);
  }
  return moves;
}

// this kindof becomes "removeillegalmoves" now.
vector<Square *>
ChessPiece::removeMovesLeadingToSelfCheck(vector<Square *> destinations) {
  vector<Square *> safeDestinations;
  safeDestinations.reserve(50);

  for (Square *destination : destinations) {
    Move moveToTry = Move(board, location, destination, true);
    board->doMove(&moveToTry);

    if (!board->isInCheck(color) && !board->tooMuchRepetition()) {
      safeDestinations.push_back(destination);
    }

    board->undoMove();
  }
  return safeDestinations;
}

PieceType ChessPiece::getType() const {
  return type;
}

Square *ChessPiece::getLocation() const {
  return location;
}

void ChessPiece::setLocation(Square *location) {
  ChessPiece::location = location;
}

PieceColor ChessPiece::getColor() const {
  return color;
}

int ChessPiece::getAmountOfSteps() const {
  return amountOfSteps;
}

void ChessPiece::increaseAmountOfSteps(int amount) {
  ChessPiece::amountOfSteps += amount;
}

void ChessPiece::decreaseAmountOfSteps(int amount) {
  ChessPiece::amountOfSteps -= amount;
}
const string &ChessPiece::getName() const {
  return name;
}
char ChessPiece::getLetter() const {
  return letter;
}
void ChessPiece::setLetter(char letter) {
  ChessPiece::letter = letter;
}
