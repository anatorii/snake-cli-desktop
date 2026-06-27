#pragma once

#include <iostream>

#include "brick_game.h"
#include "defines.h"
#include "utils.h"

class GameModel {
   private:
    GameInfo_t game_info;
    int** field;
    int key;

   public:
    GameModel() : field(nullptr), key(-1) {
        // Initialization of the initial state
        game_info = updateCurrentState();
        field = game_info.field;
    }

    ~GameModel() {
        // Freeing up resources if necessary
    }

    // Getters for data
    GameInfo_t getGameInfo() const { return game_info; }
    int** getField() const { return field; }
    int getKey() const { return key; }

    // Setters
    void setKey(int newKey) { key = newKey; }

    // Game state update
    void updateState() {
        game_info = updateCurrentState();
        field = game_info.field;
    }

    // Handling user input
    void processInput() {
        int action = input_to_action(key);
        if ((action > -1 && !isPaused()) || (action == Pause)) {
            userInput((UserAction_t)action, 0);
        }
        key = -1;
    }

    // Checking the game state
    bool isPaused() const { return game_info.pause == 1; }
    bool isGameOver() const { return game_info.speed == 0; }
};
