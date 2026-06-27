// game_controller.h
#pragma once
#include "game_model.h"
#include "game_view.h"

class GameController {
   private:
    GameModel model;
    GameView *view;
    guint timeout_id;
    GtkApplication *app;

    // Event handlers
    static gboolean onTimeout(gpointer user_data);
    static gboolean onKeyPressed(GtkEventControllerKey *controller, guint keyval, guint keycode,
                                 GdkModifierType state, gpointer user_data);

    void updateView();
    void handleInput();

   public:
    GameController(GtkApplication *app);
    ~GameController();

    void run();
    void stop();

    // Methods for interacting with GTK
    static void activate(GtkApplication *app, gpointer user_data);
};