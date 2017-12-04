//
// Created by Lukas Bos on 30/11/2017.
//

#include "Board.h"
#include "chesspieces/Rook.h"
#include "chesspieces/Bishop.h"
#include "chesspieces/Knight.h"
#include "chesspieces/Pawn.h"
#include "chesspieces/Queen.h"
#include "chesspieces/King.h"

using namespace sf;

Board::Board(sf::RenderWindow& window) : window(window) {
    createBoard();
}

void Board::drawBoard() {
    //draw the gridElements
    for (short i  = 0; i < 8; i++) {
        for (short j = 0; j < 8; j++) {
            elements[i][j]->drawGridElement(window);
        }
    }
}

// set all pieces to the initial position
void Board::startGame(PieceColor bottomColor, PieceColor topColor) {
    createBoard();

    this->bottomColor = bottomColor;
    this->topColor = topColor;

    //@TODO make the queens switch place dependent on color

    // add all pieces at the top of the board
    elements[0][0]->setChessPiece(new Rook(this, elements[0][0], topColor));
    elements[1][0]->setChessPiece(new Knight(this, elements[1][0], topColor));
    elements[2][0]->setChessPiece(new Bishop(this, elements[2][0], topColor));
    elements[3][0]->setChessPiece(new Queen(this, elements[3][0], topColor));
    elements[4][0]->setChessPiece(new King(this, elements[4][0], topColor));
    elements[5][0]->setChessPiece(new Bishop(this, elements[5][0], topColor));
    elements[6][0]->setChessPiece(new Knight(this, elements[6][0], topColor));
    elements[7][0]->setChessPiece(new Rook(this, elements[7][0], topColor));

    // add pawns
    for (int i = 0; i < 8; i++) {
        elements[i][1]->setChessPiece(new Pawn(this, elements[i][1], topColor));
        elements[i][6]->setChessPiece(new Pawn(this, elements[i][6], bottomColor));
    }

    // add all pieces at the bottom of the board
    elements[0][7]->setChessPiece(new Rook(this, elements[0][7], bottomColor));
    elements[1][7]->setChessPiece(new Knight(this, elements[1][7], bottomColor));
    elements[2][7]->setChessPiece(new Bishop(this, elements[2][7], bottomColor));
    elements[3][7]->setChessPiece(
            new Queen(this, elements[3][7], bottomColor));
    elements[4][7]->setChessPiece(
            new King(this, elements[4][7], bottomColor));
    elements[5][7]->setChessPiece(new Bishop(this, elements[5][7], bottomColor));
    elements[6][7]->setChessPiece(new Knight(this, elements[6][7], bottomColor));
    elements[7][7]->setChessPiece(new Rook(this, elements[7][7], bottomColor));
}

void Board::selectGridElementFromMousePos(int x, int y) {
    for (short i  = 0; i < 8; i++) {
        for (short j = 0; j < 8; j++) {
            GridElement *element = elements[i][j];
            element->setSelected(false); // there should be no other elements be selected.



            if (x > element->posX && x < element->posX+BLOCK_SIZE
                    && y > element->posY && y < element->posY+BLOCK_SIZE) {
                // now we have the clicked element

                // when we click on a focused element
                if (element->isFocused && !element->chessPiece) {
                    //move the piece to there

                    element->chessPiece = selectedGridElement->chessPiece;
                    element->chessPiece->location = element;
                    element->chessPiece->hasMoved = true;

                    selectedGridElement->chessPiece = NULL;
                } else {
                    element->setSelected(true);
                    selectedGridElement = element;
                }

            }
        }
    }

    focusGridElements();
}

void Board::selectGridElementFromCoordinates(sf::Vector2i coordinates) {
    selectedGridElement->setSelected(false); // set selected property of previous element to false
    selectedGridElement = elements[(coordinates.x)][coordinates.y];
    selectedGridElement->setSelected(true); // set new element to selected

    focusGridElements();
}

void Board::focusGridElements() {
    for (short i = 0; i < 8; i++) {
        for (short j = 0; j < 8; j++) {
            elements[i][j]->setFocused(false); // there should be no other elements be selected.
        }
    }

    // here check for the elements where the selected element may move to
    if (selectedGridElement && selectedGridElement->chessPiece) {
        std::vector<GridElement *> availableMoves = selectedGridElement->chessPiece->getAvailableMoves();

        for (int i = 0; i < availableMoves.size(); i++) {
            availableMoves.at(i)->setFocused(true);
        }
    }
}

void Board::createBoard() {
    for (short i = 0; i < 8; i++) {
        for (short j = 0; j < 8; j++) {
            Color color = (j + i) % 2 == 1 ? Color(100, 100, 100) : Color::White;

            sf::Vector2i coordinates = sf::Vector2i(i, j);
            elements[i][j] = new GridElement(i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, color, coordinates);
        }
    }
}
