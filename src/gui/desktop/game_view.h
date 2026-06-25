#pragma once

#include <gtk/gtk.h>
#include <string>
#include "brick_game.h"
#include "defines.h"

class GameView {
private:
    // GTK виджеты
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *level_value;
    GtkWidget *score_value;
    GtkWidget *speed_value;
    GtkWidget *high_score_value;
    GtkWidget *status_label;
    GtkWidget **cells;
    GtkWidget **nextFigure;
    
    // Константы
    static constexpr int GRID_ROWS = HEIGHT;
    static constexpr int GRID_COLS = WIDTH;
    static constexpr int CELL_SIZE = 40;
    
    // Вспомогательные методы
    std::string intToColor(int icolor) const;
    GdkRGBA* getCellColor(GtkWidget *cell) const;
    void drawField(int** field, const GameInfo_t& info);
    static void onDrawCell(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);
    GtkWidget* createCell(int row, int col);
    void setupUI();
    
public:
    GameView(GtkApplication *app);
    ~GameView();
    
    // Основные методы отображения
    void render(int** field, const GameInfo_t& info);
    void show();
    void hide();
    
    // Установка обработчиков
    void setKeyHandler(void* controller);
    
    // Геттеры для виджетов
    GtkWidget* getWindow() const { return window; }
    
    // Статический метод для создания view
    static GameView* create(GtkApplication *app);
};