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
        // Инициализация начального состояния
        game_info = updateCurrentState();
        field = game_info.field;
    }
    
    ~GameModel() {
        // Освобождение ресурсов если необходимо
    }
    
    // Геттеры для данных
    GameInfo_t getGameInfo() const { return game_info; }
    int** getField() const { return field; }
    int getKey() const { return key; }
    
    // Сеттеры
    void setKey(int newKey) { key = newKey; }
    
    // Обновление состояния игры
    void updateState() {
        game_info = updateCurrentState();
        field = game_info.field;
    }
    
    // Обработка пользовательского ввода
    void processInput() {
        int action = input_to_action(key);
        if ((action > -1 && !isPaused()) || (action == Pause)) {
            userInput((UserAction_t)action, 0);
        }
        key = -1;
    }
    
    // Проверка состояния игры
    bool isPaused() const { return game_info.pause == 1; }
    bool isGameOver() const { return game_info.speed == 0; }
};
