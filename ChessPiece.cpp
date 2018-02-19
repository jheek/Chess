//
// Created by Lukas Bos on 30/11/2017.
//

#include <iostream>
#include "ChessPiece.h"

using namespace sf;

ChessPiece::ChessPiece(Board *board, GridElement *location, PieceColor color)
        : board(board), location(location), color(color) {
}

void ChessPiece::drawChessPiece(sf::RenderWindow &window) {
    texture.loadFromImage(img);
    sprite.setTexture(texture, true);
    sprite.setPosition(sf::Vector2f(location->posX + 10, location->posY + 10)); // absolute position
    window.draw(sprite);
}

std::vector<GridElement *> ChessPiece::getAvailableMoves(bool considerOtherPieces, bool considerCheck) {
    // virtual method for child classes
}

int ChessPiece::getLocationScore(int x, int y) {

    // if we are the topPlayer we must reverse the scores
    if (board->bottomPlayer->color == color) {
        y = 7 - y;
    }

    // check first y then x.
    return locationScores.at(y).at(x);
}

std::vector<GridElement *>
ChessPiece::calculateMovesForDirections(GridElement *location, Vector2i directions[], Board *board,
                                        PieceColor color, short amountOfDirections, short maxAmountOfSteps,
                                        bool considerOtherPieces, bool considerCheck) {

    /* considerOtherPieces
     * with considerOtherpieces == true the location of a chesspiece is also stored in the vector
     * Otherwise only the path towards it is stored.
     *
     * ConsiderCheck states if we should check if the moves are valid (thus aren't setting the own king check)
     */


    std::vector<GridElement *> moves;
    int y = location->coordinates.y;
    int x = location->coordinates.x;

    for (int i = 0; i < amountOfDirections; i++) {
        for (int j = 1; j <= maxAmountOfSteps; j++) {
            int xLocation = x + (j * directions[i].x);
            int yLocation = y + (j * directions[i].y);
            bool elementExists = xLocation >= 0 && xLocation < 8 && yLocation >= 0 && yLocation < 8;
            if (elementExists) {
                GridElement *element = board->elements[xLocation][yLocation];
                if (element->chessPiece && element->chessPiece->color == color) {

                    // if we have to check for mate we must be able to travel one further...
                    // todo make names more descirptive
                    if (!considerOtherPieces) {
                        moves.push_back(element);
                    }
                    break;
                } else if (element->chessPiece) {
                    moves.push_back(element);
                    break;
                }
                moves.push_back(element);
            }
        }
    }

    return removeMovesLeadingToSelfCheck(moves, considerCheck);
}

std::vector<GridElement *>
ChessPiece::removeMovesLeadingToSelfCheck(std::vector<GridElement *> destinations, bool considerCheck) {
    std::vector<GridElement *> safeDestinations;
    for (GridElement *destination : destinations) {
        Move *moveToTry = new Move(location, destination);
        board->doMove(moveToTry);

        if (considerCheck) {
            if (board->checkedGridElement) board->checkedGridElement->isChecked = false;
            board->checkedKing = NULL;

            board->searchForCheckedKing();
        }

        // this also works by setting check upon the other king :p
        // quite spectacular tactic I would say


        // if there is no king checked or if it is the other king...
        if (!board->checkedKing || board->checkedKing->color != color) {

            safeDestinations.push_back(destination);

        }
        board->undoMove();
    }
    return safeDestinations;
}

void ChessPiece::generateImage(PieceType type) {
    // for the finding images
    std::string names[] = {"King", "Queen", "Rook", "Bishop", "Knight", "Pawn"};
    imageUrlPrefix = color == BLACK ? "Black" : "White";
    img.loadFromFile("images/" + imageUrlPrefix + names[type] + ".png");
}
