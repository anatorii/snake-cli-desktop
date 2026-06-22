#include "lib.hpp"

#include <iostream>

#include "game.hpp"

extern "C" {

void userInput(UserAction_t action, bool hold) {
    (void)hold;
    Game *game = Game::getInstance();
    switch (action) {
        case Start:
            game->start();
            break;
        case Left:
            game->rotate(Left_Direction);
            break;
        case Right:
            game->rotate(Right_Direction);
            break;
        case Up:
            game->rotate(Up_Direction);
            break;
        case Down:
            game->rotate(Down_Direction);
            break;
        case Action:
            game->speedUp();
            break;
        case Pause:
            // std::cout << "Pause" << std::endl;
            break;
        case Terminate:
            // std::cout << "Terminate" << std::endl;
            break;
    }
}

GameInfo_t updateCurrentState() {
    Game *game = Game::getInstance();
    GameInfo_t info = game->update();
    return info;
}
}
