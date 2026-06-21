#ifndef DEFINES_HPP
#define DEFINES_HPP

#define WIDTH 10
#define HEIGHT 20
#define START_LENGTH 4
#define START_DIRECTION 0
#define START_X 5
#define START_Y 20
#define MAX_LENGTH 200
#define TIME_QUANT 20
#define START_LEVEL 1
#define MAX_LEVELS 10
#define APPLE_SCORE 5
#define LEVEL_SCORE 5
#define START_SPEED 1
#define SCORE_FILE "score_snake"
#define TIME_QUANT 20

enum { Left_Rotation = -1, Right_Rotation = 1 };

enum {
    Up_Direction = 0,
    Right_Direction = 1,
    Down_Direction = 2,
    Left_Direction = 3,
};

#endif
