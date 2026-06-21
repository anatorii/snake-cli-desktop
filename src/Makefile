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
SNAKE_LDFLAGS = -lncurses -lm -lstdc++
TEST_LIBS = -lcheck -lm -lpthread -lsubunit
SANITIZE = -fsanitize=address
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Имена библиотек и исполняемых файлов
TETRIS_LIB = libtetris.so
SNAKE_LIB = libsnake.so
TETRIS_BIN = tetris
SNAKE_BIN = snake

# Исходники
TETRIS_SRC = $(wildcard $(SRCDIR)/brick_game/tetris/*.c)
SNAKE_SRC = $(wildcard $(SRCDIR)/brick_game/snake/*.cpp)
GUI_SRC = $(wildcard $(SRCDIR)/gui/cli/*.c)

# Объектные файлы
TETRIS_OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(TETRIS_SRC))
SNAKE_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SNAKE_SRC))
GUI_OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(GUI_SRC))

# Заголовочные файлы (для зависимостей)
TETRIS_HEAD = $(wildcard $(SRCDIR)/brick_game/tetris/*.h)
SNAKE_HEAD = $(wildcard $(SRCDIR)/brick_game/snake/*.hpp)
GUI_HEAD = $(wildcard $(SRCDIR)/gui/cli/*.h)

# Основные цели
all: directories $(TETRIS_BIN) $(SNAKE_BIN)

# Создание необходимых директорий
directories:
	@mkdir -p $(BINDIR) $(OBJDIR)/brick_game/tetris $(OBJDIR)/brick_game/snake $(OBJDIR)/gui/cli

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

# =============== ИСПОЛНЯЕМЫЕ ФАЙЛЫ ===============

# Исполняемый файл Tetris
$(TETRIS_BIN): $(GUI_OBJ) $(TETRIS_LIB)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(GUI_OBJ) -L$(BINDIR) -l:$(TETRIS_LIB) $(LDFLAGS) -Wl,-rpath,'$$ORIGIN'

# Исполняемый файл Snake
$(SNAKE_BIN): $(GUI_OBJ) $(SNAKE_LIB)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(GUI_OBJ) -L$(BINDIR) -l:$(SNAKE_LIB) $(SNAKE_LDFLAGS) -Wl,-rpath,'$$ORIGIN'

# =============== ЗАПУСК ===============

run-tetris: $(TETRIS_BIN)
	cd $(BINDIR) && ./$(TETRIS_BIN)

run-snake: clean $(SNAKE_BIN)
	cd $(BINDIR) && ./$(SNAKE_BIN)

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

.PHONY: all clean cf directories docs dist run-tetris run-snake debug test
