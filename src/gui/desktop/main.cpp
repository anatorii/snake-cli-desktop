#include "game_controller.h"

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.brick_game.mvc", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(GameController::activate), nullptr);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}