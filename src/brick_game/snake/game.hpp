#ifndef GAME_H
#define GAME_H

#include <functional>
#include <map>
#include <memory>
#include <queue>

#include "app_path.hpp"
#include "defines.hpp"
#include "lib.hpp"
#include "snake_game_fsm.hpp"

namespace s21 {

class SnakeGameFsm;

struct Cell {
    int x, y;
    bool operator<(const Cell& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

class Game {
   public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    static Game* getInstance();
    GameInfo_t update();
    void start();
    void rotate(int dir);
    void speedUp();
    void setSpeed(int speed);
    void updatePause();
    void countScore();

   private:
    Game();
    ~Game();
    static Game* instance;
    int** field = nullptr;
    int** next = nullptr;
    int score = 0;
    int high_score = 0;
    int pause = 0;
    int level = 1;
    int snakeSpeed = 1;
    int snakeLength = 4;
    int direction = Up_Direction;
    std::vector<Cell> snake;
    Cell apple;
    std::unique_ptr<SnakeGameFsm> fsm;
    int ticks = 0;
    AppPath app_path;

   public:
    void initGame();
    void finishGame();
    void moveSnake();
    void rotateSnake(int dir);
    void speedUpSnake();
    void addTail();
    void putApple(int x = -1, int y = -1);
    bool checkApple();
    bool nextWall();
    bool nextSelf();
    void cleanField();
    void checkTime();
    bool maxLength();
    bool storeScore(int score);
    void loadScore();
};

}  // namespace s21

#endif
