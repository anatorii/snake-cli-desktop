#include "game_controller.h"
#include <gtk/gtk.h>

GameController::GameController(GtkApplication *app) : app(app), view(nullptr), timeout_id(0) {
    view = GameView::create(app);
}

GameController::~GameController() {
    stop();
    delete view;
}

gboolean GameController::onTimeout(gpointer user_data) {
    GameController* controller = static_cast<GameController*>(user_data);
    
    // Обновляем состояние модели
    controller->model.updateState();
    
    // Обрабатываем ввод
    controller->handleInput();
    
    // Обновляем представление
    controller->updateView();
    
    return G_SOURCE_CONTINUE;
}

gboolean GameController::onKeyPressed(GtkEventControllerKey *controller, 
                                      guint keyval, guint keycode, 
                                      GdkModifierType state, 
                                      gpointer user_data) {
    GameController* gameController = static_cast<GameController*>(user_data);
    gameController->model.setKey(keyval);
    return TRUE;
}

void GameController::updateView() {
    GameInfo_t info = model.getGameInfo();
    int** field = model.getField();
    view->render(field, info);
}

void GameController::handleInput() {
    model.processInput();
}

void GameController::run() {
    // Настраиваем обработчики клавиш
    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(onKeyPressed), this);
    gtk_widget_add_controller(view->getWindow(), key_controller);
    
    // Запускаем таймер
    timeout_id = g_timeout_add(20, onTimeout, this);
    
    // Показываем окно
    view->show();
}

void GameController::stop() {
    if (timeout_id) {
        g_source_remove(timeout_id);
        timeout_id = 0;
    }
}

void GameController::activate(GtkApplication *app, gpointer user_data) {
    GameController* controller = new GameController(app);
    controller->run();
    
    // Сохраняем контроллер в данных приложения для очистки
    g_object_set_data_full(G_OBJECT(app), "controller", controller, [](gpointer data) { delete static_cast<GameController*>(data); });
}