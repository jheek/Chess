#include <SFML/Graphics.hpp>
#include "Board.h"
#include "players/HumanPlayer.h"
#include "players/MinMaxPlayer.h"

using namespace sf;

int main() {

    // set the context
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Chess", Style::Default, settings);

    // draw the board & init game
    Board board;
    Player *bottomPlayer = new HumanPlayer(WHITE, window);
    Player *topPlayer = new HumanPlayer(BLACK, window);
    board.startGame(bottomPlayer, topPlayer);
    Player *currentPlayer;

    // white begins
    currentPlayer = bottomPlayer->color == WHITE ? bottomPlayer : topPlayer;

    //start interface
    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Color menuColor(220, 220, 220); // light gray
            window.clear(menuColor);

            Move *nextMove = currentPlayer->getNextMove(board);
            if (nextMove) {
                board.doMove(nextMove);
                board.checkGameStatus();

                // switch player after move
                if (currentPlayer == topPlayer) {
                    currentPlayer = bottomPlayer;
                } else {
                    currentPlayer = topPlayer;
                }
            }

            board.drawBoard(window); // this is where the redraw of the board happens

            // use keys to set window modi.
            // When having a humanplayer it is recommended to make the human the bottomplayer

            if (sf::Keyboard::isKeyPressed(Keyboard::W)) { // humanplayer plays as White
                Player *bottomPlayer = new HumanPlayer(WHITE, window);
                Player *topPlayer = new MinMaxPlayer(BLACK);
                board.startGame(bottomPlayer, topPlayer);

            } else if (Keyboard::isKeyPressed(Keyboard::B)) {  // humanplayer plays as Black
                Player *bottomPlayer = new HumanPlayer(BLACK, window);
                Player *topPlayer = new MinMaxPlayer(WHITE);
                board.startGame(bottomPlayer, topPlayer);
            } else if (Keyboard::isKeyPressed(Keyboard::R)) {
                board.undoMove();
            }

            Font font;
            Font boldFont;
            font.loadFromFile("OpenSans-Regular.ttf");
            boldFont.loadFromFile("OpenSans-Bold.ttf");

            GridElement *selectedElement = board.selectedGridElement;
            if (selectedElement) {
                Vector2i newCoordinates = selectedElement->coordinates;
                board.selectGridElementFromCoordinates(newCoordinates);

                Text elementName(selectedElement->name, font, 26);
                elementName.setFillColor(Color::Black);
                elementName.setCharacterSize(32);
                elementName.setPosition(BOARD_SIZE + 40, WINDOW_HEIGHT / 2 + 40);

                window.draw(elementName);
            }

            Text currentMoveText("Current move", font, 26);
            currentMoveText.setFillColor(Color::Black);
            currentMoveText.setCharacterSize(24);
            currentMoveText.setPosition(BOARD_SIZE + 40, WINDOW_HEIGHT / 2 - 50);

            Text currentColorToMoveName(currentPlayer->color == BLACK ? "Black" : "White", boldFont, 26);
            currentColorToMoveName.setFillColor(Color::Black);
            currentColorToMoveName.setCharacterSize(48);
            currentColorToMoveName.setPosition(BOARD_SIZE + 40, WINDOW_HEIGHT / 2 - 20);


            Text topPlayerTypeText(board.topPlayer->getType(), font, 26);
            topPlayerTypeText.setFillColor(Color::Black);
            topPlayerTypeText.setCharacterSize(24);
            topPlayerTypeText.setPosition(BOARD_SIZE + 40, 40);

            Text bottomPlayerTypeText(board.bottomPlayer->getType(), font, 26);
            bottomPlayerTypeText.setFillColor(Color::Black);
            bottomPlayerTypeText.setCharacterSize(24);
            bottomPlayerTypeText.setPosition(BOARD_SIZE + 40, WINDOW_HEIGHT - 80);


            window.draw(topPlayerTypeText);
            window.draw(bottomPlayerTypeText);
            window.draw(currentMoveText);
            window.draw(currentColorToMoveName);
            window.display();
        }
    }
    return 0;
}
