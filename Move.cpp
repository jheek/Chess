//
// Created by Lukas Bos on 11/12/2017.
//

#include "Move.h"


Move::Move() {}

Move::Move(GridElement *startOfMove, GridElement *endOfMove) : startOfMove(startOfMove), endOfMove(endOfMove) {}

Move *Move::getReverseMove() {
    return new Move(this->endOfMove, this->startOfMove);
}