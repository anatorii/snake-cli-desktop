#include "game_view.h"

#include <gtk/gtk.h>

#include <format>
#include <iostream>

GameView::GameView(GtkApplication* app) : app(app), window(nullptr), cells(nullptr), nextFigure(nullptr) {
    level_value = nullptr;
    score_value = nullptr;
    speed_value = nullptr;
    high_score_value = nullptr;
    status_label = nullptr;
    setupUI();
}

GameView::~GameView() {
    invalidateWidgets();
    if (cells) {
        g_free(cells);
        cells = nullptr;
    }
    if (nextFigure) {
        g_free(nextFigure);
        nextFigure = nullptr;
    }
}

std::string GameView::intToColor(int icolor) const {
    switch (icolor) {
        case 1:
            return "white";
        case 2:
            return "blue";
        case 3:
            return "magenta";
        case 4:
            return "green";
        case 5:
            return "cyan";
        case 6:
            return "red";
        case 7:
            return "yellow";
        default:
            return "black";
    }
}

void GameView::render(int** field, const GameInfo_t& info) {
    if (!widgets_valid) {
        g_debug("Widgets are invalid, skipping render");
        return;
    }

    auto setLabelText = [](GtkWidget* widget, const std::string& text) {
        if (widget && GTK_IS_LABEL(widget)) {
            gtk_label_set_text(GTK_LABEL(widget), text.c_str());
        }
    };

    // Обновляем текстовые метки
    if (level_value && GTK_IS_LABEL(level_value)) {
        gtk_label_set_text(GTK_LABEL(level_value), std::to_string(info.level).c_str());
    }
    if (score_value && GTK_IS_LABEL(score_value)) {
        gtk_label_set_text(GTK_LABEL(score_value), std::to_string(info.score).c_str());
    }
    if (info.speed > 0) {
        if (speed_value && GTK_IS_LABEL(speed_value)) {
            gtk_label_set_text(GTK_LABEL(speed_value), std::to_string(info.speed).c_str());
        }
    }
    if (high_score_value && GTK_IS_LABEL(high_score_value)) {
        gtk_label_set_text(GTK_LABEL(high_score_value), std::to_string(info.high_score).c_str());
    }

    // Обновляем статус
    if (status_label && GTK_IS_LABEL(status_label)) {
        if (info.pause == 1) {
            gtk_label_set_markup(GTK_LABEL(status_label), "<b><big>PAUSE</big></b>");
        } else if (info.speed == 0) {
            gtk_label_set_markup(GTK_LABEL(status_label), "<b><big>GAME OVER</big></b>");
        } else if (info.speed == -1) {
            gtk_label_set_markup(GTK_LABEL(status_label), "<b><big>WIN!</big></b>");
        } else {
            gtk_label_set_text(GTK_LABEL(status_label), "");
        }
    }

    // Обновляем цвета клеток
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            std::string scolor = intToColor(field[row][col]);
            GdkRGBA current_color;
            gdk_rgba_parse(&current_color, scolor.c_str());

            GdkRGBA* color = getCellColor(cells[row * GRID_COLS + col]);
            if (color) {
                *color = current_color;
            }
        }
    }

    // Перерисовываем все клетки
    if (cells) {
        for (int i = 0; i < GRID_ROWS * GRID_COLS; i++) {
            if (cells[i] && GTK_IS_WIDGET(cells[i])) {
                gtk_widget_queue_draw(cells[i]);
            }
        }
    }

    if (info.next != nullptr) {
        // Обновляем цвета клеток след фигуры
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                std::string scolor = intToColor(info.next[row][col]);
                GdkRGBA current_color;
                gdk_rgba_parse(&current_color, scolor.c_str());

                GdkRGBA* color = getCellColor(nextFigure[row * 4 + col]);
                if (color) {
                    *color = current_color;
                }
            }
        }
        // Перерисовываем все клетки след фигуры
        if (nextFigure) {
            for (int i = 0; i < 4 * 4; i++) {
                if (nextFigure[i] && GTK_IS_WIDGET(nextFigure[i])) {
                    gtk_widget_queue_draw(nextFigure[i]);
                }
            }
        }
    }
}

void GameView::setupUI() {
    // Создаем окно
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Brick Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    // Основной контейнер
    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(main_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_box, GTK_ALIGN_CENTER);

    // --- Левый GtkFixed (игровое поле) ---
    GtkWidget* left_fixed = gtk_fixed_new();
    gtk_widget_set_halign(left_fixed, GTK_ALIGN_START);
    gtk_widget_set_valign(left_fixed, GTK_ALIGN_START);
    gtk_widget_set_size_request(left_fixed, 300, 600);

    // Создаем GtkGrid для игрового поля (20x20 для змейки)
    GtkWidget* game_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(game_grid), 1);
    gtk_grid_set_column_spacing(GTK_GRID(game_grid), 1);
    gtk_grid_set_baseline_row(GTK_GRID(game_grid), 5);
    gtk_widget_set_margin_top(game_grid, 5);
    gtk_widget_set_margin_bottom(game_grid, 5);
    gtk_widget_set_margin_start(game_grid, 5);
    gtk_widget_set_margin_end(game_grid, 5);

    // Создаем клетки
    cells = g_new0(GtkWidget*, GRID_ROWS * GRID_COLS);

    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            GtkWidget* cell = createCell(row, col);
            cells[row * GRID_COLS + col] = cell;

            // Устанавливаем рамку для ячейки
            GtkWidget* frame = gtk_frame_new(NULL);
            gtk_frame_set_child(GTK_FRAME(frame), cell);
            gtk_grid_attach(GTK_GRID(game_grid), frame, col, row, 1, 1);
        }
    }

    // Помещаем сетку в левый GtkFixed
    gtk_fixed_put(GTK_FIXED(left_fixed), game_grid, 0, 0);

    // --- Правый GtkFixed (информационная панель) ---
    GtkWidget* right_fixed = gtk_fixed_new();
    gtk_widget_set_size_request(right_fixed, 300, -1);

    // Создаем информационную сетку
    GtkWidget* info_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(info_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(info_grid), 10);

    // Информация об уровне
    GtkWidget* level_label = gtk_label_new("LEVEL:");
    gtk_label_set_markup(GTK_LABEL(level_label), "<b><big>LEVEL:</big></b>");
    gtk_widget_set_halign(level_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(info_grid), level_label, 0, 1, 1, 1);
    level_value = gtk_label_new("0");
    gtk_widget_set_halign(level_value, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(info_grid), level_value, 0, 2, 1, 1);

    // Информация о счете
    GtkWidget* score_label = gtk_label_new("SCORE:");
    gtk_label_set_markup(GTK_LABEL(score_label), "<b><big>SCORE:</big></b>");
    gtk_widget_set_halign(score_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(info_grid), score_label, 0, 3, 1, 1);
    score_value = gtk_label_new("0");
    gtk_widget_set_halign(score_value, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(info_grid), score_value, 0, 4, 1, 1);

    // Информация о скорости
    GtkWidget* speed_label = gtk_label_new("SPEED:");
    gtk_label_set_markup(GTK_LABEL(speed_label), "<b><big>SPEED:</big></b>");
    gtk_widget_set_halign(speed_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(info_grid), speed_label, 0, 5, 1, 1);
    speed_value = gtk_label_new("0");
    gtk_widget_set_halign(speed_value, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(info_grid), speed_value, 0, 6, 1, 1);

    // Информация о максимальном счете
    GtkWidget* high_score_label = gtk_label_new("HIGH SCORE:");
    gtk_label_set_markup(GTK_LABEL(high_score_label), "<b><big>HIGH SCORE:</big></b>");
    gtk_widget_set_halign(high_score_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(info_grid), high_score_label, 0, 7, 1, 1);
    high_score_value = gtk_label_new("0");
    gtk_widget_set_halign(high_score_value, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(info_grid), high_score_value, 0, 8, 1, 1);

    // Информация о следующей фигуре
    GtkWidget* next_label = gtk_label_new("NEXT:");
    gtk_label_set_markup(GTK_LABEL(next_label), "<b><big>NEXT:</big></b>");
    gtk_widget_set_halign(next_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(info_grid), next_label, 0, 9, 1, 1);

    // Создаем GtkGrid для следующей фигуры
    GtkWidget* next_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(next_grid), 1);
    gtk_grid_set_column_spacing(GTK_GRID(next_grid), 1);
    gtk_grid_set_baseline_row(GTK_GRID(next_grid), 5);
    gtk_widget_set_margin_top(next_grid, 5);
    gtk_widget_set_margin_bottom(next_grid, 5);
    gtk_widget_set_margin_start(next_grid, 5);
    gtk_widget_set_margin_end(next_grid, 5);

    // Создаем клетки для следующей фигуры
    nextFigure = g_new0(GtkWidget*, 4 * 4);
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            GtkWidget* cell = createCell(row, col);
            nextFigure[row * 4 + col] = cell;

            // Устанавливаем рамку для ячейки
            GtkWidget* frame = gtk_frame_new(NULL);
            gtk_frame_set_child(GTK_FRAME(frame), cell);
            gtk_grid_attach(GTK_GRID(next_grid), frame, col, row, 1, 1);
        }
    }
    gtk_grid_attach(GTK_GRID(info_grid), next_grid, 0, 11, 1, 1);

    // Информация о статусе игры
    status_label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(status_label), "");
    gtk_widget_set_halign(status_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(info_grid), status_label, 0, 12, 1, 1);

    // Помещаем информационную сетку в правый GtkFixed
    gtk_fixed_put(GTK_FIXED(right_fixed), info_grid, 10, 10);

    // Добавляем оба контейнера в главный бокс
    gtk_box_append(GTK_BOX(main_box), left_fixed);
    gtk_box_append(GTK_BOX(main_box), right_fixed);

    // Добавляем главный бокс в окно
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    g_signal_connect(window, "destroy", G_CALLBACK(onWindowDestroy), this);

    widgets_valid = true;
}

void GameView::onWindowDestroy(GtkWidget* widget, gpointer user_data) {
    GameView* view = static_cast<GameView*>(user_data);
    view->invalidateWidgets();
}

void GameView::invalidateWidgets() {
    widgets_valid = false;
    level_value = nullptr;
    score_value = nullptr;
    speed_value = nullptr;
    high_score_value = nullptr;
    status_label = nullptr;
    window = nullptr;
}

void GameView::onDrawCell(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
    GdkRGBA* color = (GdkRGBA*)g_object_get_data(G_OBJECT(area), "color");

    // Заливаем клетку цветом
    gdk_cairo_set_source_rgba(cr, color);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    // Рисуем границу
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_set_line_width(cr, 1);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_stroke(cr);
}

GtkWidget* GameView::createCell(int row, int col) {
    GtkWidget* drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, CELL_SIZE, CELL_SIZE);

    // Устанавливаем начальный цвет (белый)
    GdkRGBA white = {0, 0, 0, 1.0};
    GdkRGBA* color = gdk_rgba_copy(&white);
    g_object_set_data_full(G_OBJECT(drawing_area), "color", color, (GDestroyNotify)gdk_rgba_free);
    g_object_set_data(G_OBJECT(drawing_area), "row", GINT_TO_POINTER(row));
    g_object_set_data(G_OBJECT(drawing_area), "col", GINT_TO_POINTER(col));

    // Обработчик рисования
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area),
                                   (GtkDrawingAreaDrawFunc)GameView::onDrawCell, NULL, NULL);

    // Делаем клетку фокусируемой для клавиатуры
    gtk_widget_set_focusable(drawing_area, TRUE);
    gtk_widget_set_can_focus(drawing_area, TRUE);

    return drawing_area;
}

GdkRGBA* GameView::getCellColor(GtkWidget* cell) const {
    if (!cell || !GTK_IS_WIDGET(cell)) {
        return nullptr;
    }
    GdkRGBA* color = (GdkRGBA*)g_object_get_data(G_OBJECT(cell), "color");
    return color;
}

GameView* GameView::create(GtkApplication* app) { return new GameView(app); }

void GameView::show() { gtk_window_present(GTK_WINDOW(window)); }

void GameView::hide() { gtk_window_close(GTK_WINDOW(window)); }
