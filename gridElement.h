//
// Created by Lukas Bos on 30/11/2017.
//

#ifndef CHESS_GRIDELEMENT_H
#define CHESS_GRIDELEMENT_H

#include <SFML/Graphics.hpp>
#include "ChessPiece.h"

class GridElement {

public:
    GridElement();
    GridElement(int x, int y, int size, sf::Color color);
    sf::RectangleShape rectangle;
    ChessPiece chessPiece;
    void drawGridElement(sf::RenderWindow &window);
private:
    int posX;
    int posY;
    int size;
    sf::Color color;
};


#endif //CHESS_GRIDELEMENT_H
