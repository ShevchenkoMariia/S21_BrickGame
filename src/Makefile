CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
CHECK_FLAGS = -lcheck -pthread -lsubunit -lm
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -L. brick_game.a 

.PHONY: all clean 

all: tetris clean

brick_game.a:
	@$(CC) $(CFLAGS) -c brick_game/tetris/tetris.c -o tetris.o
	@ar rc $@ tetris.o
	@ranlib $@

tetris: main.c gui/cli/cli.c brick_game.a
	@$(CC) $(CFLAGS) $^ -lncurses -lm $(LDFLAGS) -o tetris
	@./tetris

test: brick_game.a brick_game/tetris/tests/tests.c brick_game/tetris/tetris.c
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) $(CHECK_FLAGS) $(GCOV_FLAGS) -o tests
	@./tests

gcov_report: test
	@lcov -c -d . -o coverage.info
	@genhtml coverage.info -o gcov_report
	@open gcov_report/index.html

valgrind: brick_game.a
	@$(CC) $(CFLAGS) brick_game/tetris/tests/tests.c brick_game/tetris/tetris.c $(LDFLAGS) $(CHECK_FLAGS)
	@valgrind --tool=memcheck --leak-check=yes ./a.out

check_style:
	@cp ../materials/linters/.clang-format .
	@clang-format -n *.c brick_game/tetris/tests/tests.c brick_game/tetris/tetris.c brick_game/tetris/tetris.h gui/cli/cli.c  gui/cli/cli.h
	@rm .clang-format

style:
	@cp ../materials/linters/.clang-format .
	@clang-format -i brick_game/tetris/tests/tests.c brick_game/tetris/tetris.c brick_game/tetris/tetris.h gui/cli/cli.c  gui/cli/cli.h  
	@rm .clang-format

g_style:
	@clang-format --style=Google -n *.c brick_game/tetris/tests/tests.c brick_game/tetris/tetris.c brick_game/tetris/tetris.h gui/cli/cli.c  gui/cli/cli.h


clean:
	@rm -r *.o brick_game.a *.gcda *.gcno coverage.info tests gcov_report
