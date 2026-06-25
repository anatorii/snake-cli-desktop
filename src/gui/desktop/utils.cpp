#include "utils.h"

#include <gtk/gtk.h>
#include <string.h>
#include "brick_game.h"
#include "defines.h"

int input_to_action(int input) {
    int action = -1;

    if (input == GDK_KEY_Left)
        action = Left;
    else if (input == GDK_KEY_Right)
        action = Right;
    else if (input == GDK_KEY_Up)
        action = Up;
    else if (input == GDK_KEY_Down)
        action = Down;
    else if (input == GDK_KEY_KP_Space || input == GDK_KEY_space)
        action = Action;
    else if (input == GDK_KEY_q)
        action = Terminate;
    else if (input == GDK_KEY_KP_Enter || input == GDK_KEY_Return)
        action = Start;
    else if (input == GDK_KEY_p)
        action = Pause;
    else
        action = -1;

    return action;
}
