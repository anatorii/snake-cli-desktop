#include <gtest/gtest.h>
#include <ncurses.h>

#include <string>

#include "../gui/cli/utils.h"
#include "test_snake.hpp"

using namespace std;

void run_fsm(int reps = 50) {
    for (size_t i = 0; i < reps; i++) {
        GameInfo_t game_info = updateCurrentState();
    }
}

TEST(snake_tests, start_test) {
    int act = -1;

    act = input_to_action('\n');
    userInput((UserAction_t)act, 0);
    run_fsm();
}

TEST(snake_tests, apple_test) {
    int act = -1;

    act = input_to_action('\n');
    userInput((UserAction_t)act, 0);
    run_fsm(200);
}

TEST(snake_tests, speed_up_test) {
    int act = -1;

    act = input_to_action('\n');
    userInput((UserAction_t)act, 0);
    run_fsm();

    act = input_to_action(32);
    userInput((UserAction_t)act, 0);
    run_fsm(100);
}

TEST(snake_tests, rotation_test) {
    int act = -1;

    act = input_to_action('\n');
    userInput((UserAction_t)act, 0);
    run_fsm();

    act = input_to_action(KEY_LEFT);
    userInput((UserAction_t)act, 0);
    run_fsm(100);

    act = input_to_action(KEY_DOWN);
    userInput((UserAction_t)act, 0);
    run_fsm(100);

    act = input_to_action(KEY_RIGHT);
    userInput((UserAction_t)act, 0);
    run_fsm(100);

    act = input_to_action(KEY_UP);
    userInput((UserAction_t)act, 0);
    run_fsm(100);
}

TEST(snake_tests, wall_collision_test) {
    int act = -1;

    act = input_to_action('\n');
    userInput((UserAction_t)act, 0);
    run_fsm(2000);
}
