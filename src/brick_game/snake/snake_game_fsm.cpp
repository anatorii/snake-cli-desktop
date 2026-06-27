#include "snake_game_fsm.hpp"

#include <bits/chrono.h>

#include <iostream>
#include <random>
#include <set>

// ////////////////////////////////////////////////////
// конструктор SnakeGameFsm
// ////////////////////////////////////////////////////

s21::SnakeGameFsm::SnakeGameFsm(Game* g) : game(g) {
    initStateMap();
    currentState = State_Start;
}

// ////////////////////////////////////////////////////
// определение событий
// ////////////////////////////////////////////////////

void s21::SnakeGameFsm::timer() { externalEvent(transitions[Event_Timer][getCurrentState()], nullptr); }

void s21::SnakeGameFsm::start() { externalEvent(transitions[Event_Start][getCurrentState()], nullptr); }

void s21::SnakeGameFsm::rotate(RotateData* data) {
    externalEvent(transitions[Event_Rotate][getCurrentState()], data);
}

void s21::SnakeGameFsm::speedUp() { externalEvent(transitions[Event_SpeedUp][getCurrentState()], nullptr); }

// ////////////////////////////////////////////////////
// определение функций состояния
// ////////////////////////////////////////////////////

void s21::SnakeGameFsm::StartAction(SnakeEventData* data) { game->initGame(); }

void s21::SnakeGameFsm::FinishAction(SnakeEventData* data) { game->finishGame(); }

void s21::SnakeGameFsm::MoveAction(SnakeEventData* data) {
    if (game->nextSelf()) {
        internalEvent(State_Self);
    } else if (game->nextWall()) {
        internalEvent(State_Wall);
    } else {
        game->moveSnake();
        if (game->checkApple()) {
            internalEvent(State_Apple);
        }
    }
}

void s21::SnakeGameFsm::WinAction(SnakeEventData* data) {
    game->setSpeed(-1);
    internalEvent(State_Finish);
}

void s21::SnakeGameFsm::FailAction(SnakeEventData* data) {
    game->setSpeed(0);
    internalEvent(State_Finish);
}

void s21::SnakeGameFsm::RotateAction(RotateData* data) { game->rotateSnake(data->direction); }

void s21::SnakeGameFsm::ActionAction(SnakeEventData* data) { game->speedUpSnake(); }

void s21::SnakeGameFsm::AppleAction(SnakeEventData* data) {
    game->countScore();
    game->addTail();
    if (!game->maxLength()) {
        game->putApple();
    } else {
        internalEvent(State_Win);
    }
}

void s21::SnakeGameFsm::WallAction(SnakeEventData* data) { internalEvent(State_Fail); }

void s21::SnakeGameFsm::SelfAction(SnakeEventData* data) { internalEvent(State_Fail); }

// ////////////////////////////////////////////////////
// инициализация состояний
// ////////////////////////////////////////////////////

void s21::SnakeGameFsm::initStateMap() {
    stateMap[State_Start] = createState([this](EventData* e) { this->StartAction((SnakeEventData*)e); },
                                        nullptr, nullptr, nullptr);
    stateMap[State_Finish] = createState([this](EventData* e) { this->FinishAction((SnakeEventData*)e); },
                                         nullptr, nullptr, nullptr);
    stateMap[State_Move] = createState([this](EventData* e) { this->MoveAction((SnakeEventData*)e); },
                                       nullptr, nullptr, nullptr);
    stateMap[State_Win] =
        createState([this](EventData* e) { this->WinAction((SnakeEventData*)e); }, nullptr, nullptr, nullptr);
    stateMap[State_Fail] = createState([this](EventData* e) { this->FailAction((SnakeEventData*)e); },
                                       nullptr, nullptr, nullptr);
    stateMap[State_Rotate] =
        createState([this](EventData* e) { this->RotateAction((RotateData*)e); }, nullptr, nullptr, nullptr);
    stateMap[State_Action] = createState([this](EventData* e) { this->ActionAction((SnakeEventData*)e); },
                                         nullptr, nullptr, nullptr);
    stateMap[State_Apple] = createState([this](EventData* e) { this->AppleAction((SnakeEventData*)e); },
                                        nullptr, nullptr, nullptr);
    stateMap[State_Wall] = createState([this](EventData* e) { this->WallAction((SnakeEventData*)e); },
                                       nullptr, nullptr, nullptr);
    stateMap[State_Self] = createState([this](EventData* e) { this->SelfAction((SnakeEventData*)e); },
                                       nullptr, nullptr, nullptr);
}
