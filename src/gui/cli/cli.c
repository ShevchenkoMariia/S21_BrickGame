#include "cli.h"

win feild, info, arena, nextFigure, score, level, record, control;

void renderingGame(GameInfo_t* game) {
  if (!game->pause && isRunning && !exitGame) {
    nodelay(feild.window, TRUE);
    fullClearWin(&control);
    for (int i = 1, y = 1; i < HEIGHT - 1; i++, y++) {
      for (int j = 1, x = 1; j < WIDTH - 1; j++, x += 2) {
        mvwaddch(feild.window, y, x,
                 COLOR_PAIR(game->field[i][j]) | A_INVIS | ' ');
        mvwaddch(feild.window, y, x + 1,
                 COLOR_PAIR(game->field[i][j]) | A_INVIS | ' ');
      }
    }
    winNextFigure(game);
    winScore(game);
    winLevel(game);
    winRecord(game);

    wrefresh(feild.window);
  } else if (game->pause) {
    nodelay(feild.window, FALSE);
    infoControl();
  }
}

void infoControl() {
  if (gameOver) {
    renderingBoldStr(&feild, 9, " GAME OVER ", 4);
    renderingBoldStr(&feild, 10, " restart -> Enter ", 4);

    gameOver = false;
  }
  keyControl();
  box(control.window, 0, 0);
  renderingBoldStr(&control, 0, "control", 8);
}

void keyControl() {
  wattron(control.window, COLOR_PAIR(8));
  mvwprintw(control.window, 1, 1, "START | Enter");
  mvwprintw(control.window, 2, 1, "PAUSE | p");
  mvwprintw(control.window, 3, 1, "<- -> | a | d");
  mvwprintw(control.window, 4, 1, "DOWN  | s");
  mvwprintw(control.window, 5, 1, "TURN  | w");
  mvwprintw(control.window, 6, 1, "EXIT  | Esc");
  wattroff(control.window, COLOR_PAIR(8));
  wrefresh(control.window);
}

void winRecord(GameInfo_t* game) {
  int rec = readingRecord();
  int count = countDigits(rec);
  wattron(record.window, COLOR_PAIR(7));
  mvwprintw(record.window, 1, (record.columns - count - 2) / 2, " %d ", rec);
  wattroff(record.window, COLOR_PAIR(7));
  if (game->score > rec) saveRecord(game->score);
  wrefresh(record.window);
}

int readingRecord() {
  int num = 0;
  // файл чтения
  char* filename = "record.txt";
  // чтение из файла
  FILE* fp = fopen(filename, "r");
  if (fp != NULL) {
    fscanf(fp, "%d", &num);
    fclose(fp);
  }
  return num;
}

void saveRecord(int num) {
  char* filename = "record.txt";
  FILE* fp = fopen(filename, "w");
  if (fp != NULL) {
    fprintf(fp, "%d", num);
    fclose(fp);
  }
}

void winScore(GameInfo_t* game) {
  int count = countDigits(game->score);
  wattron(score.window, COLOR_PAIR(7));
  mvwprintw(score.window, 1, (score.columns - count - 2) / 2, " %d ",
            game->score);
  wattroff(score.window, COLOR_PAIR(7));
  wrefresh(score.window);
}

void winLevel(GameInfo_t* game) {
  int count = countDigits(game->level);
  wattron(level.window, COLOR_PAIR(7));
  mvwprintw(level.window, 1, (level.columns - count - 2) / 2, " %d ",
            game->level);
  wattroff(level.window, COLOR_PAIR(7));
  wrefresh(level.window);
}

int countDigits(int num) {
  if (num == 0) {
    return 1;
  }
  return log10(abs(num)) + 1;
}

void initializingColor() {
  if (start_color() != ERR) {
    init_pair(0, COLOR_BLACK, COLOR_BLACK);
    init_color(COLOR_RED, 700, 0, 0);
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_color(COLOR_CYAN, 999, 450, 0);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);  //оранжевый
    init_color(COLOR_YELLOW, 900, 800, 0);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);
    init_color(COLOR_GREEN, 0, 550, 0);
    init_pair(4, COLOR_WHITE, COLOR_GREEN);
    init_color(COLOR_BLUE, 200, 0, 900);
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    init_color(COLOR_MAGENTA, 900, 0, 500);
    init_pair(6, COLOR_WHITE, COLOR_MAGENTA);  //розовый
    init_color(COLOR_WHITE, 500, 0, 700);
    init_pair(7, COLOR_WHITE, COLOR_WHITE);   //фиолетовый
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);  //фиолетовый
  } else {
    fprintf(stderr, "The terminal does not support color\n");
  }
}

void initializingWin(win* window, int rows, int columns, int y, int x) {
  window->rows = rows;
  window->columns = columns;
  window->y = y;
  window->x = x;
  window->window = newwin(window->rows, window->columns, window->y, window->x);
  box(window->window, 0, 0);
  wrefresh(window->window);
}

void renderingBoldStr(win* window, int numStr, const char str[], int color) {
  for (int i = 0, x = (int)((window->columns - (int)strlen(str)) / 2);
       i < (int)strlen(str); i++, x++) {
    mvwaddch(window->window, numStr, x, COLOR_PAIR(color) | A_BOLD | str[i]);
  }
  wrefresh(window->window);
}

void initializingGameWin() {
  initscr();
  curs_set(0);
  noecho();
  initializingColor();
  initializingWin(&arena, HEIGHT + 2, WIDTH * 3 + 7, 0, 0);
  initializingWin(&feild, HEIGHT, WIDTH * 2 - 2, 1, 2);
  initializingWin(&nextFigure, 5, WIDTH + 4, 1, WIDTH * 2 + 1);
  renderingBoldStr(&nextFigure, 0, "next", 8);
  initializingWin(&score, 3, WIDTH + 4, 6, WIDTH * 2 + 1);
  renderingBoldStr(&score, 0, "score", 8);
  initializingWin(&level, 3, WIDTH + 4, 9, WIDTH * 2 + 1);
  renderingBoldStr(&level, 0, "level", 8);
  initializingWin(&record, 3, WIDTH + 4, 12, WIDTH * 2 + 1);
  renderingBoldStr(&record, 0, "record", 8);
  initializingWin(&control, 8, WIDTH + 4, 15, WIDTH * 2 + 1);
  infoControl();
  keypad(feild.window, TRUE);
}

void clearWin(win* window) {
  for (int i = 1; i < window->rows - 1; i++) {
    for (int j = 1; j < window->columns - 1; j++) {
      mvwaddch(window->window, i, j, ' ');
    }
  }
  wrefresh(window->window);
}

void fullClearWin(win* window) {
  for (int i = 0; i < window->rows; i++) {
    for (int j = 0; j < window->columns; j++) {
      mvwaddch(window->window, i, j, ' ');
    }
  }
  wrefresh(window->window);
}

void winNextFigure(GameInfo_t* game) {
  for (int i = 1; i < nextFigure.rows - 1; i++) {
    for (int j = 1; j < nextFigure.columns - 1; j++) {
      mvwaddch(nextFigure.window, i, j, ' ');
    }
  }
  for (int i = 0, y = 2; i < next.figure.rows; i++, y++) {
    for (int j = 0, x = (nextFigure.columns - next.figure.columns * 2) / 2;
         j < next.figure.columns; j++, x += 2) {
      mvwaddch(nextFigure.window, y, x,
               COLOR_PAIR(game->next[i][j]) | A_BOLD | ' ');
      mvwaddch(nextFigure.window, y, x + 1,
               COLOR_PAIR(game->next[i][j]) | A_BOLD | ' ');
    }
  }
  box(nextFigure.window, 0, 0);
  renderingBoldStr(&nextFigure, 0, "next", 8);
  wrefresh(nextFigure.window);
}

void delWins() {
  delwin(arena.window);
  delwin(info.window);
  delwin(feild.window);
  delwin(nextFigure.window);
  delwin(score.window);
  delwin(level.window);
  delwin(record.window);
  delwin(control.window);
}
