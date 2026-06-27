#include "game.hpp"

#include <bits/chrono.h>

#include <iostream>
#include <random>
#include <set>

#include "defines.hpp"
#include "state_machine.hpp"

s21::Game::Game() : fsm(std::make_unique<s21::SnakeGameFsm>(this)) {
    initGame();

    int** field = new int*[HEIGHT];
    for (int i = 0; i < HEIGHT; i++) {
        field[i] = new int[WIDTH];
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = 0;
        }
    }
    this->field = field;
}

s21::Game::~Game() {
    if (field) {
        for (int i = 0; i < HEIGHT; i++) {
            delete[] field[i];
        }
        delete[] field;
    }
}

s21::Game* s21::Game::getInstance() {
    static s21::Game instance;
    return &instance;
}

GameInfo_t s21::Game::update() {
    if (pause == 0) {
        checkTime();
    }

    cleanField();
    field[apple.y][apple.x] = 6;
    field[snake[0].y][snake[0].x] = 7;
    for (size_t i = 1; i < snake.size(); i++) {
        field[snake[i].y][snake[i].x] = 4;
    }

    GameInfo_t info;
    info.field = field;
    info.next = next;
    info.score = score;
    info.high_score = high_score;
    info.level = level;
    info.speed = snakeSpeed;
    info.pause = pause;

    return info;
}

void s21::Game::start() { fsm->start(); }

void s21::Game::rotate(int dir) {
    s21::RotateData* pData = new s21::RotateData(dir);
    fsm->rotate(pData);
}

void s21::Game::speedUp() { fsm->speedUp(); }

void s21::Game::setSpeed(int speed) { snakeSpeed = speed; }

void s21::Game::updatePause() {
    if (this->pause != 0) {
        this->pause = 0;
    } else {
        this->pause = 1;
    }
}

void s21::Game::checkTime() {
    // 100ms - 10 speed
    // 1000ms - 1 speed
    ticks++;
    int level_ticks = (100 / TIME_QUANT - 1000 / TIME_QUANT) * (snakeSpeed - 1) / 9 + 1000 / TIME_QUANT;
    if (ticks > level_ticks) {
        fsm->timer();
        ticks = 0;
    }
}

bool s21::Game::maxLength() { return snake.size() == MAX_LENGTH; }

void s21::Game::initGame() {
    ticks = 0;
    snake.push_back(s21::Cell{5, 15});
    snake.push_back(s21::Cell{5, 16});
    snake.push_back(s21::Cell{5, 17});
    snake.push_back(s21::Cell{5, 18});
    putApple(5, 14);
}

void s21::Game::finishGame() {}

void s21::Game::moveSnake() {
    s21::Cell head;

    for (size_t i = 0; i < snake.size(); i++) {
        if (i == 0) {
            head = snake[0];
            if (direction == Up_Direction) {
                snake[0].y--;
            } else if (direction == Down_Direction) {
                snake[0].y++;
            } else if (direction == Left_Direction) {
                snake[0].x--;
            } else {
                snake[0].x++;
            }
        } else {
            s21::Cell tmp = snake[i];
            snake[i] = head;
            head = tmp;
        }
    }
}

void s21::Game::rotateSnake(int dir) {
    int rotation = 0;
    if (direction == Up_Direction) {
        if (dir == Left_Direction)
            rotation = -1;
        else if (dir == Right_Direction)
            rotation = 1;
    } else if (direction == Down_Direction) {
        if (dir == Left_Direction)
            rotation = 1;
        else if (dir == Right_Direction)
            rotation = -1;
    } else if (direction == Left_Direction) {
        if (dir == Down_Direction)
            rotation = -1;
        else if (dir == Up_Direction)
            rotation = 1;
    } else {
        if (dir == Down_Direction)
            rotation = 1;
        else if (dir == Up_Direction)
            rotation = -1;
    }
    direction += rotation;
    if (direction < 0) direction = 3;
    if (direction > 3) direction = 0;
}

void s21::Game::speedUpSnake() {
    if (snakeSpeed < 10) snakeSpeed++;
}

void s21::Game::addTail() {
    int i = snake.size() - 1;
    int dx = snake[i].x - snake[i - 1].x;
    int dy = snake[i].y - snake[i - 1].y;
    snake.push_back(s21::Cell{snake[i].x + dx, snake[i].y + dy});
}

void s21::Game::putApple(int x, int y) {
    if (x != -1 && y != -1) {
        apple = s21::Cell{x, y};
        return;
    }

    std::set<s21::Cell> field;
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            field.insert(s21::Cell{x, y});
        }
    }

    for (size_t i = 0; i < snake.size(); i++) {
        field.erase(snake[i]);
    }

    std::vector<s21::Cell> cells(field.begin(), field.end());

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, cells.size() - 1);
    apple = cells[dist(gen)];
}

bool s21::Game::checkApple() {
    s21::Cell head = snake[0];
    return head.x == apple.x && head.y == apple.y;
}

bool s21::Game::nextWall() {
    s21::Cell head = snake[0];
    if (direction == Up_Direction) {
        head.y--;
    } else if (direction == Down_Direction) {
        head.y++;
    } else if (direction == Left_Direction) {
        head.x--;
    } else {
        head.x++;
    }
    return head.x < 0 || head.x >= 10 || head.y < 0 || head.y >= 20;
}

bool s21::Game::nextSelf() {
    s21::Cell head = snake[0];
    if (direction == Up_Direction) {
        head.y--;
    } else if (direction == Down_Direction) {
        head.y++;
    } else if (direction == Left_Direction) {
        head.x--;
    } else {
        head.x++;
    }
    for (size_t i = 0; i < snake.size(); i++) {
        if (head.x == snake[i].x && head.y == snake[i].y) {
            return true;
        }
    }
    return false;
}

void s21::Game::cleanField() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = 0;
        }
    }
}

void s21::Game::countScore() {
    score++;

    int calcLevel = score / LEVEL_SCORE + 1;
    if (level < MAX_LEVELS && level < calcLevel) {
        level++;
        if (snakeSpeed < 10) {
            snakeSpeed++;
        }
    }

    if (high_score < score) {
        high_score = score;
        // storeScore(high_score);
    }
}
