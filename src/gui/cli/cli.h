#ifndef CLI_H
#define CLI_H

#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

#include "../../brick_game/tetris/tetris.h"

typedef struct window {
  int rows;
  int columns;
  int y;
  int x;
  WINDOW* window;
} win;

extern win feild, info, arena, nextFigure, score, level, record, control;
extern bool isRunning;
extern bool exitGame;
extern bool gameOver;

void initializingGameWin();
void clearWin(win* window);
void initializingWin(win* window, int rows, int columns, int y, int x);
void renderingBoldStr(win* window, int numStr, const char str[], int color);
void initializingColor();
void renderingGame(GameInfo_t* game);
void winNextFigure(GameInfo_t* game);
void winScore(GameInfo_t* game);
void winLevel(GameInfo_t* game);
void winRecord(GameInfo_t* game);
int countDigits(int num);
int readingRecord();
void saveRecord(int num);
void infoControl();
void keyControl();
void fullClearWin(win* window);
void delWins();

#endif
