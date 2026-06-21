#ifndef SNAKE_GAME_FSM_H
#define SNAKE_GAME_FSM_H

#include <functional>
#include <memory>
#include <queue>

#include "game.hpp"
#include "state_machine.hpp"

class Game;

class SnakeEventData : public EventData {
   public:
    SnakeEventData(int k) : key(k){};
    int key;
};

class RotateData : public EventData {
   public:
    RotateData(int d) : direction(d){};
    int direction;
};

class SnakeGameFsm : public StateMachine {
   public:
    SnakeGameFsm(Game* g);
    // события вызываемые снаружи
    void timer();
    void start();
    void rotate(RotateData* data);
    void speedUp();

   private:
    Game* game;

    enum EventEnum { Event_Timer = 0, Event_Start, Event_Rotate, Event_SpeedUp, Event_Number };

    enum StateEnum {
        State_Start = 0,
        State_Finish,
        State_Move,
        State_Win,
        State_Fail,
        State_Rotate,
        State_Action,
        State_Apple,
        State_Wall,
        State_Self,
        State_Number
    };

    //                 State_Start, State_Finish,State_Move,  State_Win,
    //                 State_Fail,  State_Rotate,State_Action,State_Apple,
    //                 State_Wall,  State_Self
    // Event_Timer     Event_Ignore,Event_Ignore,State_Move,
    // Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore
    // Event_Start     State_Move,
    // Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore
    // Event_Rotate
    // Event_Ignore,Event_Ignore,State_Rotate,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore
    // Event_SpeedUp
    // Event_Ignore,Event_Ignore,State_Action,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore,Event_Ignore

    static constexpr std::array<std::array<short, State_Number>, Event_Number> transitions = {
        {{{Event_Ignore, Event_Ignore, State_Move, Event_Ignore, Event_Ignore, State_Move, State_Move,
           State_Move, Event_Ignore, Event_Ignore}},
         {{State_Move, Event_Ignore, Event_Ignore, Event_Ignore, Event_Ignore, Event_Ignore, Event_Ignore,
           Event_Ignore, Event_Ignore, Event_Ignore}},
         {{Event_Ignore, Event_Ignore, State_Rotate, Event_Ignore, Event_Ignore, Event_Ignore, Event_Ignore,
           State_Rotate, Event_Ignore, Event_Ignore}},
         {{Event_Ignore, Event_Ignore, State_Action, Event_Ignore, Event_Ignore, Event_Ignore, Event_Ignore,
           Event_Ignore, Event_Ignore, Event_Ignore}}}};

    // функци состояния
    void StartAction(SnakeEventData* data);
    void FinishAction(SnakeEventData* data);
    void MoveAction(SnakeEventData* data);
    void WinAction(SnakeEventData* data);
    void FailAction(SnakeEventData* data);
    void RotateAction(RotateData* data);
    void ActionAction(SnakeEventData* data);
    void AppleAction(SnakeEventData* data);
    void WallAction(SnakeEventData* data);
    void SelfAction(SnakeEventData* data);

    // инициализация состояний
    void initStateMap();
};

#endif
