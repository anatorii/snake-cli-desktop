#include "snake_game_fsm.hpp"

#include <bits/chrono.h>

#include <iostream>
#include <random>
#include <set>

// ////////////////////////////////////////////////////
// конструктор SnakeGameFsm
// ////////////////////////////////////////////////////

SnakeGameFsm::SnakeGameFsm(Game* g) : game(g) {
    initStateMap();
    currentState = State_Start;
}

// ////////////////////////////////////////////////////
// определение событий
// ////////////////////////////////////////////////////

void SnakeGameFsm::timer() {
    // std::cout << "SnakeGameFsm::timer" << std::endl;
    externalEvent(transitions[Event_Timer][getCurrentState()], nullptr);
}

void SnakeGameFsm::start() {
    // std::cout << "SnakeGameFsm::start" << std::endl;
    externalEvent(transitions[Event_Start][getCurrentState()], nullptr);
}

void SnakeGameFsm::rotate(RotateData* data) {
    // std::cout << "SnakeGameFsm::rotate" << std::endl;
    externalEvent(transitions[Event_Rotate][getCurrentState()], data);
}

void SnakeGameFsm::speedUp() {
    // std::cout << "SnakeGameFsm::speedUp" << std::endl;
    externalEvent(transitions[Event_SpeedUp][getCurrentState()], nullptr);
}

// ////////////////////////////////////////////////////
// определение функций состояния
// ////////////////////////////////////////////////////

void SnakeGameFsm::StartAction(SnakeEventData* data) {
    // std::cout << "Action start" << std::endl;
    game->initGame();
}

void SnakeGameFsm::FinishAction(SnakeEventData* data) {
    // std::cout << "Action finish" << std::endl;
    game->finishGame();
}

void SnakeGameFsm::MoveAction(SnakeEventData* data) {
    // std::cout << "Action move" << std::endl;
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

void SnakeGameFsm::WinAction(SnakeEventData* data) {
    // std::cout << "Action win" << std::endl;
    internalEvent(State_Finish);
}

void SnakeGameFsm::FailAction(SnakeEventData* data) {
    // std::cout << "Action fail" << std::endl;
    game->setSpeed(0);
    internalEvent(State_Finish);
}

void SnakeGameFsm::RotateAction(RotateData* data) {
    // std::cout << "Action rotate" << std::endl;
    game->rotateSnake(data->direction);
}

void SnakeGameFsm::ActionAction(SnakeEventData* data) {
    // std::cout << "Action action" << std::endl;
    game->speedUpSnake();
}

void SnakeGameFsm::AppleAction(SnakeEventData* data) {
    // std::cout << "Action apple" << std::endl;
    game->countScore();
    game->addTail();
    game->putApple();
}

void SnakeGameFsm::WallAction(SnakeEventData* data) {
    // std::cout << "Action wall" << std::endl;
    internalEvent(State_Fail);
}

void SnakeGameFsm::SelfAction(SnakeEventData* data) {
    // std::cout << "Action self" << std::endl;
    internalEvent(State_Fail);
}

// ////////////////////////////////////////////////////
// инициализация состояний
// ////////////////////////////////////////////////////

void SnakeGameFsm::initStateMap() {
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
