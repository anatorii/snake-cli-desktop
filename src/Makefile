CC = gcc
CCP = g++
CFLAGS = -std=c11 -fPIC
CPPFLAGS = -std=c++17 -fPIC
# CFLAGS = -std=c11 -Werror -Wall -Wextra -fPIC
# CPPFLAGS = -std=c++17 -Werror -Wall -Wextra -fPIC

SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin
DISTDIR = ../dist
DOCDIR = ../docs

LDFLAGS = -lncurses -lm
TEST_LIBS = -lcheck -lm -lpthread -lsubunit
SNAKE_LDFLAGS = -lm -lncurses
SNAKE_TEST_LIBS = -lgtest -lpthread
SANITIZE = -fsanitize=address
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -O0 -std=c++20 -fPIC
GTKFLAGS = `pkg-config --cflags --libs gtk4`


# Имена библиотек и исполняемых файлов
TETRIS_LIB = libtetris.so
SNAKE_LIB = libsnake.so
TETRIS_BIN = tetris
SNAKE_BIN = snake
TETRIS_DESKTOP = tetris_desktop
SNAKE_DESKTOP = snake_desktop

# Исходники
TETRIS_SRC = $(wildcard $(SRCDIR)/brick_game/tetris/*.c)
SNAKE_SRC = $(wildcard $(SRCDIR)/brick_game/snake/*.cpp)
GUI_SRC = $(wildcard $(SRCDIR)/gui/cli/*.c)
DESKTOP_SRC = $(wildcard $(SRCDIR)/gui/desktop/*.cpp)
TEST_SRC = $(wildcard $(SRCDIR)/tests_snake/*.cpp)

# Объектные файлы
TETRIS_OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(TETRIS_SRC))
SNAKE_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SNAKE_SRC))
GUI_OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(GUI_SRC))
DESKTOP_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(DESKTOP_SRC))
TEST_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(TEST_SRC))

# Заголовочные файлы (для зависимостей)
TETRIS_HEAD = $(wildcard $(SRCDIR)/brick_game/tetris/*.h)
SNAKE_HEAD = $(wildcard $(SRCDIR)/brick_game/snake/*.hpp)
GUI_HEAD = $(wildcard $(SRCDIR)/gui/cli/*.h)
DESKTOP_HEAD = $(wildcard $(SRCDIR)/gui/desktop/*.h)

# Основные цели
all: directories $(TETRIS_BIN) $(SNAKE_BIN)

# Создание необходимых директорий
directories:
	@mkdir -p $(BINDIR) $(OBJDIR)/brick_game/tetris $(OBJDIR)/brick_game/snake $(OBJDIR)/gui/cli $(OBJDIR)/gui/desktop $(OBJDIR)/tests_snake

# Очистка
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DISTDIR) $(DOCDIR)

# Форматирование кода
cf:
	@find $(SRCDIR) -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) -exec clang-format -i {} \;

# =============== БИБЛИОТЕКИ ===============

# Библиотека Tetris (C)
$(TETRIS_LIB): $(TETRIS_OBJ) | directories
	$(CC) -shared -o $(BINDIR)/$@ $^ $(LDFLAGS)

# Библиотека Snake (C++)
$(SNAKE_LIB): $(SNAKE_OBJ) | directories
	$(CCP) -shared -o $(BINDIR)/$@ $^ $(LDFLAGS)

# =============== ОБЪЕКТНЫЕ ФАЙЛЫ ===============

# Объектные файлы Tetris
$(OBJDIR)/brick_game/tetris/%.o: $(SRCDIR)/brick_game/tetris/%.c $(TETRIS_HEAD) | directories
	$(CC) $(CFLAGS) -c $< -o $@

# Объектные файлы Snake
$(OBJDIR)/brick_game/snake/%.o: $(SRCDIR)/brick_game/snake/%.cpp $(SNAKE_HEAD) | directories
	$(CCP) $(CPPFLAGS) -c $< -o $@

# Объектные файлы GUI (общие)
$(OBJDIR)/gui/cli/%.o: $(SRCDIR)/gui/cli/%.c $(GUI_HEAD) | directories
	$(CC) $(CFLAGS) -c $< -o $@

# Объектные файлы Desktop (общие)
$(OBJDIR)/gui/desktop/%.o: $(SRCDIR)/gui/desktop/%.cpp $(DESKTOP_HEAD) | directories
	$(CCP) $(CPPFLAGS) -c $< -o $@ $(GTKFLAGS)

# Объектные файлы тестов Snake
$(OBJDIR)/tests_snake/%.o: $(SRCDIR)/tests_snake/%.cpp | directories
	$(CCP) $(CPPFLAGS) -c $< -o $@

# =============== ИСПОЛНЯЕМЫЕ ФАЙЛЫ ===============

# Исполняемый файл Tetris
$(TETRIS_BIN): $(GUI_OBJ) $(TETRIS_LIB)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(GUI_OBJ) -L$(BINDIR) -l:$(TETRIS_LIB) $(LDFLAGS) -Wl,-rpath,'$$ORIGIN'

# Исполняемый файл Snake
$(SNAKE_BIN): $(GUI_OBJ) $(SNAKE_LIB)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(GUI_OBJ) -L$(BINDIR) -l:$(SNAKE_LIB) $(SNAKE_LDFLAGS) -Wl,-rpath,'$$ORIGIN'

# Исполняемый файл Tetris desktop
$(TETRIS_DESKTOP): $(DESKTOP_OBJ) $(TETRIS_LIB)
	$(CCP) $(CPPFLAGS) -o $(BINDIR)/$@ $(DESKTOP_OBJ) -L$(BINDIR) -l:$(TETRIS_LIB) $(GTKFLAGS)-lm -Wl,-rpath,'$$ORIGIN'

# Исполняемый файл Snake desktop
$(SNAKE_DESKTOP): $(DESKTOP_OBJ) $(SNAKE_LIB)
	$(CCP) $(CPPFLAGS) -o $(BINDIR)/$@ $(DESKTOP_OBJ) -L$(BINDIR) -l:$(SNAKE_LIB) $(GTKFLAGS)-lm -Wl,-rpath,'$$ORIGIN'

gui:
	$(CCP) $(CPPFLAGS) ./gui/desktop/desktop.cpp -o ./gui/desktop/desktop.app $(GTKFLAGS) -lm
	./gui/desktop/desktop.app

# =============== ТЕСТЫ ===============

test: $(SNAKE_LIB)
	$(CCP) $(CPPFLAGS) $(TEST_SRC) gui/cli/utils.c -o $(BINDIR)/snake_test $(SNAKE_TEST_LIBS) $(SNAKE_LDFLAGS) -L$(BINDIR) -l:$(SNAKE_LIB) -Wl,-rpath,'$$ORIGIN'
	$(BINDIR)/snake_test

gcov_report:
	$(CCP) $(GCOV_FLAGS) $(TEST_SRC) $(SNAKE_SRC) gui/cli/utils.c -o $(BINDIR)/gcov_report $(SNAKE_TEST_LIBS) $(SNAKE_LDFLAGS)
	$(BINDIR)/gcov_report
	lcov --ignore-errors mismatch,unsupported,inconsistent,format,empty --rc geninfo_unexecuted_blocks=1 -c -d $(BINDIR) -o $(BINDIR)/coverage.info
	lcov -r $(BINDIR)/coverage.info 'gui' '/usr*' '**/tests_snake/*' -o $(BINDIR)/coverage.info
	genhtml --ignore-errors category -o $(BINDIR)/report $(BINDIR)/coverage.info
	xdg-open $(BINDIR)/report/index.html

# =============== ЗАПУСК ===============

run-tetris: $(TETRIS_BIN)
	cd $(BINDIR) && ./$(TETRIS_BIN)

run-snake: clean $(SNAKE_BIN)
	cd $(BINDIR) && ./$(SNAKE_BIN)

run-tetris-desktop: clean $(TETRIS_DESKTOP)
	cd $(BINDIR) && ./$(TETRIS_DESKTOP)

run-snake-desktop: clean $(SNAKE_DESKTOP)
	cd $(BINDIR) && ./$(SNAKE_DESKTOP)

# =============== ДЕБАГ ===============

debug: CFLAGS += -g -O0
debug: CPPFLAGS += -g -O0
debug: $(TETRIS_BIN) $(SNAKE_BIN)

targets:
	@echo "SRCDIR = $(SRCDIR)"
	@echo "OBJDIR = $(OBJDIR)"
	@echo "TETRIS_SRC = $(TETRIS_SRC)"
	@echo "TETRIS_OBJ = $(TETRIS_OBJ)"
	@echo "SNAKE_SRC = $(SNAKE_SRC)"
	@echo "SNAKE_OBJ = $(SNAKE_OBJ)"
	@echo "GUI_SRC = $(GUI_SRC)"
	@echo "GUI_OBJ = $(GUI_OBJ)"

.PHONY: all clean cf directories docs dist run-tetris run-snake debug test gui
