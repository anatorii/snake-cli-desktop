#pragma once

#include <gtk/gtk.h>

#include <string>

#include "brick_game.h"
#include "defines.h"

class GameView {
   private:
    // GTK widgets
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *level_value;
    GtkWidget *score_value;
    GtkWidget *speed_value;
    GtkWidget *high_score_value;
    GtkWidget *status_label;
    GtkWidget **cells;
    GtkWidget **nextFigure;
    bool widgets_valid;

    // Constants
    static constexpr int GRID_ROWS = HEIGHT;
    static constexpr int GRID_COLS = WIDTH;
    static constexpr int CELL_SIZE = 40;

    // Auxiliary methods
    std::string intToColor(int icolor) const;
    GdkRGBA *getCellColor(GtkWidget *cell) const;
    static void onDrawCell(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);
    GtkWidget *createCell(int row, int col);
    void setupUI();

   public:
    GameView(GtkApplication *app);
    ~GameView();
    void invalidateWidgets();
    static void onWindowDestroy(GtkWidget *widget, gpointer user_data);

    // Basic display methods
    void render(int **field, const GameInfo_t &info);
    void show();
    void hide();

    // Installing handlers
    // void setKeyHandler(void* controller);

    // Getters for widgets
    GtkWidget *getWindow() const { return window; }

    // Static method for creating view
    static GameView *create(GtkApplication *app);
};