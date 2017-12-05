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

std::vector<GridElement *> ChessPiece::getAvailableMoves() {
    // virtual method for child classes
}

std::vector<GridElement *>
ChessPiece::calculateMovesForDirections(GridElement *location, Vector2i directions[], Board *board,
                                        PieceColor color, short amountOfDirections, short maxAmountOfSteps) {
    std::vector<GridElement *> moves;
    short y = location->coordinates.y;
    short x = location->coordinates.x;

    for (int i = 0; i < amountOfDirections; i++) {
        for (short j = 1; j <= maxAmountOfSteps; j++) {
            short xLocation = x + (j * directions[i].x);
            short yLocation = y + (j * directions[i].y);
            bool elementExists = xLocation >= 0 && xLocation < 8 && yLocation >= 0 && yLocation < 8;
            if (elementExists) {
                GridElement *element = board->elements[xLocation][yLocation];
                if (element->chessPiece && element->chessPiece->color == color) {
                    break;
                } else if (element->chessPiece) {
                    moves.push_back(element);
                    break;
                }
                moves.push_back(element);
            }
        }
    }
    return moves;
}

void ChessPiece::generateImage(PieceType type) {
    // for the finding images
    std::string names[] = {"King", "Queen", "Rook", "Bishop", "Knight", "Pawn"};
    imageUrlPrefix = color == BLACK ? "Black" : "White";
    img.loadFromFile("images/" + imageUrlPrefix + names[type] + ".png");
}
